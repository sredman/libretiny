/* Copyright (c) Kuba Szczodrzyński 2023. */

#pragma once

#ifdef __cplusplus
// BDK's prf.h declares `struct prf_env_t * prf_env_get_from_uuid(...)` which is invalid in C++
// because prf_env_t is a typedef, not a struct tag.
// For C++, provide clean declarations and define _PRF_H_ to guard against double-inclusion.
#ifndef _PRF_H_
#define _PRF_H_

#include "rwip_config.h"

#if (BLE_PROFILES)

#include "kernel_task.h"
#include "gapm_task.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    PRF_SERVICE = 0,
    PRF_CLIENT = 1,
};

enum prf_perm_mask {
    PERM_MASK_PRF_TASK = 0x7FFF,
    PERM_POS_PRF_TASK  = 0,
    PERM_MASK_PRF_MI   = 0x8000,
    PERM_POS_PRF_MI    = 15,
};

typedef struct prf_env {
    kernel_task_id_t app_task;
    kernel_task_id_t prf_task;
} prf_env_t;

struct prf_task_env {
    struct kernel_task_desc desc;
    prf_env_t *env;
    kernel_task_id_t task;
    kernel_task_id_t id;
    uint8_t role;
};

typedef uint8_t (*prf_init_fnct)(struct prf_task_env *env, uint16_t *start_hdl, uint16_t app_task, uint8_t sec_lvl, void *params);
typedef void (*prf_destroy_fnct)(struct prf_task_env *env);
typedef void (*prf_create_fnct)(struct prf_task_env *env, uint8_t conidx);
typedef void (*prf_cleanup_fnct)(struct prf_task_env *env, uint8_t conidx, uint8_t reason);

struct prf_task_cbs {
    prf_init_fnct init;
    prf_destroy_fnct destroy;
    prf_create_fnct create;
    prf_cleanup_fnct cleanup;
};

struct prf_env_tag {
    struct prf_task_env prf[BLE_NB_PROFILES];
};

extern struct prf_env_tag prf_env;

void prf_init(uint8_t init_type);
uint8_t prf_add_profile(struct gapm_profile_task_add_cmd *params, kernel_task_id_t *prf_task);
void prf_create(uint8_t conidx);
void prf_cleanup(uint8_t conidx, uint8_t reason);
prf_env_t *prf_env_get(uint16_t prf_id);
kernel_task_id_t prf_src_task_get(prf_env_t *env, uint8_t conidx);
kernel_task_id_t prf_dst_task_get(prf_env_t *env, uint8_t conidx);
kernel_task_id_t prf_get_id_from_task(kernel_msg_id_t task);
kernel_task_id_t prf_get_task_from_id(kernel_msg_id_t id);

prf_env_t *prf_env_get_from_uuid(uint8_t conidx, uint8_t uuid_len, const uint8_t *uuid);
prf_env_t *prf_env_get_from_handle(uint8_t conidx, uint16_t handle);

#ifdef __cplusplus
}
#endif

#endif // (BLE_PROFILES)
#endif // _PRF_H_

#else
#include_next "prf.h"
#endif
