/******************************************************************************
 * GridFTP plugin for access to ceph object store
 *
 * @author Sebastien Ponce, sebastien.ponce@cern.ch
 *****************************************************************************/
#pragma once

#include "globus_gridftp_server.h"

typedef struct checksum_block_list_s {
  globus_off_t offset;
  globus_size_t size;
  unsigned int csumvalue; /* only for 32bit checksums as Adler32 or CRC32 */
  struct checksum_block_list_s *next;
} checksum_block_list_t;

typedef struct globus_l_gfs_ceph_handle_s {
  globus_mutex_t mutex;
  int fd;
  globus_result_t cached_res;
  int outstanding;
  int optimal_count;
  globus_bool_t done;
  globus_off_t blk_length;
  globus_off_t blk_offset;
  globus_size_t block_size;
  globus_gfs_operation_t op;
  /* we have to save all blocs checksums for the on the fly calculation */
  checksum_block_list_t * checksum_list;
  checksum_block_list_t *checksum_list_p;
  unsigned long number_of_blocks;
  long long fileSize;
} globus_l_gfs_ceph_handle_t;

/* a function to wrap all is needed to close a file */
static void globus_l_gfs_file_net_read_cb(globus_gfs_operation_t,
                                          globus_result_t,
                                          globus_byte_t *,
                                          globus_size_t,
                                          globus_off_t,
                                          globus_bool_t,
                                          void *);
static void globus_l_gfs_ceph_read_from_net(globus_l_gfs_ceph_handle_t * );
static globus_result_t globus_l_gfs_make_error(const char *);

int ceph_handle_open(char *, int, int, globus_l_gfs_ceph_handle_t *);
static globus_bool_t globus_l_gfs_ceph_send_next_to_client(globus_l_gfs_ceph_handle_t *);
static void globus_l_gfs_net_write_cb(globus_gfs_operation_t,
                                      globus_result_t,
                                      globus_byte_t *,
                                      globus_size_t,
                                      void *);
