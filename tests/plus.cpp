/*
  C++ interface test
*/
#include <assert.h>
#include <memcached.hh>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "server.h"

#include "test.h"

uint8_t basic_test(memcached_st *memc)
{
  Memcached foo;
  char *value_set= "This is some data";
  char *value;
  size_t value_length;

  foo.set("mine", value_set, strlen(value_set));
  value= foo.get("mine", &value_length);

  assert((memcmp(value, value_set, value_length) == 0));

  return 0;
}

test_st tests[] ={
  {"basic", 0, basic_test },
  {0, 0, 0}
};

collection_st collection[] ={
  {"block", 0, 0, tests},
  {0, 0, 0, 0}
};

#define SERVERS_TO_CREATE 1

void *world_create(void)
{
  unsigned int x;
  memcached_server_st *servers;
  server_startup_st *construct;

  construct= (server_startup_st *)malloc(sizeof(server_startup_st));
  memset(construct, 0, sizeof(server_startup_st));

  construct->count= SERVERS_TO_CREATE;
  server_startup(construct);

  return construct;
}

void world_destroy(void *p)
{
  server_startup_st *construct= (server_startup_st *)p;
  memcached_server_st *servers= (memcached_server_st *)construct->servers;
  memcached_server_list_free(servers);

  server_shutdown(construct);
  free(construct);
}

void get_world(world_st *world)
{
  world->collections= collection;
  world->create= world_create;
  world->destroy= world_destroy;
}