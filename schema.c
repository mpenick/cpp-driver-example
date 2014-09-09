/*
 This is free and unencumbered software released into the public domain.
 
 Anyone is free to copy, modify, publish, use, compile, sell, or
 distribute this software, either in source code form or as a compiled
 binary, for any purpose, commercial or non-commercial, and by any
 means.
 
 In jurisdictions that recognize copyright laws, the author or authors
 of this software dedicate any and all copyright interest in the
 software to the public domain. We make this dedication for the benefit
 of the public at large and to the detriment of our heirs and
 successors. We intend this dedication to be an overt act of
 relinquishment in perpetuity of all present and future rights to this
 software under copyright law.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
 For more information, please refer to <http://unlicense.org/>
*/

#include <stdio.h>
#include <cassandra.h>

void print_error(CassFuture* future) {
  CassString message = cass_future_error_message(future);
  fprintf(stderr, "Error: %.*s\n", (int)message.length, message.data);
}

CassError execute_query(CassSession* session, const char* query) {
  /* Create a statement that requrires 0 parameters */
  CassStatement* statement = cass_statement_new(cass_string_init(query), 0);

  /* The function cass_future_wait() doesn't have to be called */
  CassFuture* future = cass_session_execute(session, statement);
  CassError rc = cass_future_error_code(future);

  if(rc != CASS_OK) {
    print_error(future);
  }

  cass_future_free(future);
  cass_statement_free(statement);

  return rc;
}

int main() {
  /* Setup and connect to cluster */
  CassCluster* cluster = cass_cluster_new();
  cass_cluster_set_contact_points(cluster, "127.0.0.1");

  CassFuture* connect_future = cass_cluster_connect(cluster);
  cass_future_wait(connect_future);
  CassError rc = cass_future_error_code(connect_future);

  if (rc == CASS_OK) {
    CassSession* session = cass_future_get_session(connect_future);

    execute_query(session, "CREATE KEYSPACE cass_summit_2014  WITH replication = { "
                           "'class': 'SimpleStrategy', 'replication_factor': '3' };");

    execute_query(session, "USE cass_summit_2014");

    execute_query(session, "CREATE TABLE attendees (name text, job text, awesomeness bigint, "
                           "PRIMARY KEY(name)) ");

    /* Close the session */
    CassFuture* close_future = cass_session_close(session);
    cass_future_wait(close_future);
    cass_future_free(close_future);
  } else {
    /* Handle error */
    print_error(connect_future);
  }

  cass_future_free(connect_future);
  cass_cluster_free(cluster);

  return 0;
}
