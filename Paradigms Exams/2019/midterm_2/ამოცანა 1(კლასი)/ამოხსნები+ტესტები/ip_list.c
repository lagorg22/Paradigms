#include "ip_list.h"

#include <string.h>
#include <stdlib.h>

#include "packed_list.h"

#include "gl/logging/logging.h"

void IPv4Init(IPv4* ip, char a, char b, char c, char d) {
  ip->version = V4;
  ip->address[0] = a;
  ip->address[1] = b;
  ip->address[2] = c;
  ip->address[3] = d;  
}

void IPv6Init(IPv6* ip, const char* address) {
  ip->version = V6;
  ip->address = strdup(address);
}

void IPv6Dispose(IPv6* ip) {
  free(ip->address);
}

static void IPDispose(void* pt) {
  if (*(IPVersion*)pt == V6) {
    IPv6Dispose(pt);
  }
}

void IPListInit(IPList* list) {
  PackedListInit(list, IPDispose);
}

void IPListDispose(IPList* list) {
  PackedListDispose(list);
}

void IPListAppendV4(IPList* list, IPv4* v4) {
  PackedListAppend(list, v4, sizeof(IPv4));
}

void IPListAppendV6(IPList* list, IPv6* v6) {
  PackedListAppend(list, v6, sizeof(IPv6));  
}

const void* IPListGet(IPList* list, int index) {
  return PackedListGet(list, index);
}

static int CmpIP(const void* a, const void* b) {
  if (*(IPVersion*)a != *(IPVersion*)b) {
    return *(IPVersion*)a - *(IPVersion*)b;
  }
  if (*(IPVersion*)a == V4) {
    for (int i = 0; i < 4; ++i) {
      if (((IPv4*)a)->address[i] != ((IPv4*)b)->address[i]) {
	return ((IPv4*)a)->address[i] - ((IPv4*)b)->address[i];
      }
    }
    return 0;
  } else {
    return strcmp(((IPv6*)a)->address, ((IPv6*)b)->address);
  }
}

void IPListSort(IPList* list) {
  PackedListSort(list, CmpIP);
}
