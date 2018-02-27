#ifndef NPI_LNX_IPC_H
#define NPI_LNX_IPC_H

#include <rtthread.h>
#include <rthw.h>
#include <dfs_select.h>
#include <lwip/sockets.h>

#define NPI_LNX_IPC_ZLSZNP_MB_NAME      "zlsznp_mb"
#define NPI_LNX_IPC_ZLSZNP_MB_SIZE      128

#define NPI_LNX_IPC_NETMGR_MB_NAME      "netmgr_mb"
#define NPI_LNX_IPC_NETMGR_MB_SIZE      128

#define NPI_LNX_IPC_GATEWAY_MB_NAME     "gateway_mb"
#define NPI_LNX_IPC_GATEWAY_MB_SIZE     128

#define NPI_LNX_IPC_OTASERVER_MB_NAME   "otaserver_mb"
#define NPI_LNX_IPC_OTASERVER_MB_SIZE   128

extern struct rt_mailbox zlsznp_mb;
extern struct rt_mailbox netmgr_mb;
extern struct rt_mailbox gateway_mb;
extern struct rt_mailbox otaserver_mb;

#endif /* NPI_LNX_IPC_H */