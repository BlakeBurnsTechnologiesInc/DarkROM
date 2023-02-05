// SPDX-License-Identifier: GPL-2.0-only
/* vendor_hook.c
 *
 * Android Vendor Hook Support
 *
 * Copyright 2020 Google LLC
 */
#ifndef __GENKSYMS__
#include <uapi/linux/hdreg.h>
#include <net/addrconf.h>
#include <linux/fsverity.h>
#include <linux/ipc_namespace.h>
#include <linux/key-type.h>
#include <linux/mtd/mtd.h>
#include <linux/pr.h>
#include <linux/time_namespace.h>
#include <net/macsec.h>
#include <net/netfilter/nf_log.h>
#include <net/raw.h>
#include <net/smc.h>
#include <../drivers/net/wireless/intel/ipw2x00/libipw.h>
#include <../fs/mount.h>
#include <../fs/kernfs/kernfs-internal.h>
#include <../kernel/audit.h>
#include <../net/can/af_can.h>
#include <../net/tipc/bearer.h>
#include <../net/xdp/xsk_queue.h>
#include <../security/keys/internal.h>
#endif

#define CREATE_TRACE_POINTS
#include <trace/hooks/vendor_hooks.h>
#include <linux/tracepoint.h>
#include <trace/hooks/sched.h>
#include <trace/hooks/cpu.h>
#include <trace/hooks/binder.h>
#include <trace/hooks/rwsem.h>
#include <trace/hooks/futex.h>
#include <trace/hooks/dtask.h>
#include <trace/hooks/cpuidle.h>
#include <trace/hooks/topology.h>
#include <trace/hooks/mpam.h>
#include <trace/hooks/gic.h>
#include <trace/hooks/wqlockup.h>
#include <trace/hooks/debug.h>
#include <trace/hooks/sysrqcrash.h>
#include <trace/hooks/printk.h>
#include <trace/hooks/gic_v3.h>
#include <trace/hooks/epoch.h>
#include <trace/hooks/cpufreq.h>
#include <trace/hooks/fs.h>
#include <trace/hooks/i2c.h>
#include <trace/hooks/mm.h>
#include <trace/hooks/preemptirq.h>
#include <trace/hooks/ftrace_dump.h>
#include <trace/hooks/pm_domain.h>
#include <trace/hooks/bug.h>
#include <trace/hooks/timer.h>
#include <trace/hooks/softlockup.h>
#include <trace/hooks/power.h>
#include <trace/hooks/fault.h>
#include <trace/hooks/iommu.h>
#include <trace/hooks/thermal.h>
#include <trace/hooks/ufshcd.h>
#include <trace/hooks/block.h>
#include <trace/hooks/cgroup.h>
#include <trace/hooks/workqueue.h>
#include <trace/hooks/sys.h>
#include <trace/hooks/traps.h>
#include <trace/hooks/avc.h>
#include <trace/hooks/creds.h>
#include <trace/hooks/memory.h>
#include <trace/hooks/typec.h>
#include <trace/hooks/gpiolib.h>
#include <trace/hooks/signal.h>
#include <trace/hooks/logbuf.h>
#include <trace/hooks/vmscan.h>
#include <trace/hooks/psi.h>
#include <trace/hooks/selinux.h>
#include <trace/hooks/hung_task.h>
#include <trace/hooks/mmc_core.h>
#include <trace/hooks/v4l2core.h>
#include <trace/hooks/v4l2mc.h>
#include <trace/hooks/scmi.h>
#include <trace/hooks/user.h>
#include <trace/hooks/cpuidle_psci.h>
#include <trace/hooks/fips140.h>
#include <trace/hooks/remoteproc.h>
#include <trace/hooks/shmem_fs.h>
#include <trace/hooks/net.h>
#include <trace/hooks/syscall_check.h>
#include <trace/hooks/usb.h>
#include <trace/hooks/ipv6.h>
#include <trace/hooks/sound.h>
#include <trace/hooks/snd_compr.h>
#include <trace/hooks/gup.h>
#include <trace/hooks/pci.h>
#include <trace/hooks/suspend.h>

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_select_task_rq_fair);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_select_task_rq_rt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_select_task_rq_dl);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_select_fallback_rq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_refrigerator);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_scheduler_tick);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_enqueue_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_dequeue_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_can_migrate_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_find_lowest_rq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_rtmutex_prepare_setprio);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_prepare_prio_fork);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_finish_prio_fork);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_set_user_nice);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_setscheduler);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sk_alloc);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sk_free);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_nf_conn_alloc);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_nf_conn_free);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_arch_set_freq_scale);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_transaction_init);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_priority_skip);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_set_priority);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_restore_priority);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_wakeup_ilocked);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_do_send_sig_info);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_process_killed);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rwsem_init);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rwsem_wake);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rwsem_write_finished);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_alter_rwsem_list_add);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_alter_futex_plist_add);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mutex_wait_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mutex_wait_finish);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rtmutex_wait_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rtmutex_wait_finish);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rwsem_read_wait_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rwsem_read_wait_finish);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rwsem_write_wait_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rwsem_write_wait_finish);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sched_show_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_shmem_alloc_page);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpu_idle_enter);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpu_idle_exit);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mpam_set);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_find_busiest_group);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_gic_resume);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_wq_lockup_pool);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ipi_stop);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sysrq_crash);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_dump_throttled_rt_tasks);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_printk_hotplug);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_jiffies_update);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_gic_v3_set_affinity);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_gic_v3_affinity_init);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_show_suspend_epoch_val);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_show_resume_epoch_val);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_show_max_freq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_freq_table_limits);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpufreq_resolve_freq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpufreq_fast_switch);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpufreq_target);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_newidle_balance);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_nohz_balancer_kick);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_rebalance_domains);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_find_busiest_queue);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_migrate_queued_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_resume_cpus);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_find_energy_efficient_cpu);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_set_iowait);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_cpu_overutilized);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_setaffinity);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_update_cpus_allowed);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_set_skip_swapcache_flags);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_set_gfp_zone_flags);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_set_readahead_gfp_mask);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_preempt_disable);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_preempt_enable);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_irqs_disable);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_irqs_enable);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_set_task_cpu);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_fork);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_build_perf_domains);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_cpu_cgroup_attach);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_cpu_cgroup_can_attach);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_cpu_cgroup_online);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_fork_init);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_ttwu_cond);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_schedule_bug);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_exec);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ftrace_oops_enter);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ftrace_oops_exit);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ftrace_size_check);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ftrace_format_check);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ftrace_dump_buffer);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_allow_domain_state);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_map_util_freq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_update_next_freq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sugov_get_util);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_report_bug);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_em_cpu_energy);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpu_up);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpu_down);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_sched_balance_rt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_timer_calc_index);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_watchdog_timer_softlockup);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_try_to_freeze_todo);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_try_to_freeze_todo_logging);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_try_to_freeze_todo_unfrozen);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_dequeue_task_idle);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_die_kernel_fault);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_do_sea);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_do_mem_abort);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_do_sp_pc_abort);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_iommu_setup_dma_ops);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_iommu_alloc_iova);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_iommu_free_iova);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_pick_next_entity);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_check_preempt_wakeup);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cma_alloc_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cma_alloc_finish);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cma_alloc_busy_info);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_calc_alloc_flags);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mm_compaction_begin);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mm_compaction_end);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rmqueue);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_pagecache_get_page);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_filemap_fault_get_page);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_filemap_fault_cache_page);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_enable_thermal_genl_check);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_thermal_pm_notify_suspend);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_fill_prdt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_ufs_complete_init);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_ufs_reprogram_all_keys);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_prepare_command);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_update_sysfs);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_send_command);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_compl_command);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_send_uic_command);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_send_tm_command);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_check_int_errors);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_update_sdev);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ufs_clock_scaling);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_do_wake_up_sync);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_wake_flags);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_uclamp_eff_get);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_uclamp_task_util);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_uclamp_rq_util_with);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_cpufreq_transition);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cgroup_set_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_cgroup_force_kthread_migration);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_syscall_prctl_finished);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_create_worker);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_check_preempt_tick);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_check_preempt_wakeup_ignore);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_replace_next_task_fair);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_wait_for_work);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sync_txn_recvd);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_build_sched_domains);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_alter_mutex_list_add);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mutex_unlock_slowpath);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rwsem_wake_finish);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_do_undefinstr);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_bad_mode);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_arm64_serror_panic);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_selinux_avc_insert);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_selinux_avc_node_delete);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_selinux_avc_node_replace);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_selinux_avc_lookup);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_blk_alloc_rqs);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_blk_rq_ctx_init);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_commit_creds);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_exit_creds);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_override_creds);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_revert_creds);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_x);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_nx);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_ro);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_memory_rw);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_util_est_update);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_meminfo_proc_show);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_exit_mm);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_get_from_fragment_pool);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_exclude_reserved_zone);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_include_reserved_zone);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_alloc_pages_slowpath);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cma_alloc_adjust);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_show_mem);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_print_slabinfo_header);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_do_shrink_slab);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cache_show);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_typec_tcpci_override_toggling);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_typec_tcpci_chk_contaminant);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_typec_tcpci_get_vbus);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_typec_store_partner_src_caps);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_account_task_time);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_irqtime_account_process_tick);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_gpio_block_read);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ep_create_wakeup_source);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_timerfd_create);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_alloc_new_buf_locked);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_reply);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_trans);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_preset);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_proc_transaction);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_new_ref);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_del_ref);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_post_init_entity_util_avg);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_typec_tcpm_get_timer);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_typec_tcpm_adj_current_limit);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_logbuf);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_logbuf_pr_cont);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_tune_scan_type);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_tune_swappiness);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_shrink_slab_bypass);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_psi_event);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_psi_group);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_cpuset_fork);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_set_cpus_allowed_comm);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sched_setaffinity_early);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_free_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_dup_task_struct);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_after_enqueue_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_after_dequeue_task);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_enqueue_entity);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_dequeue_entity);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_entity_tick);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_enqueue_task_fair);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_dequeue_task_fair);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_prepare_update_load_avg_se);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_finish_update_load_avg_se);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_selinux_is_initialized);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_tune_inactive_ratio);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_update_topology_flags_workfn);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_of_i2c_get_board_info);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cgroup_attach);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mm_dirty_limits);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_oom_check_panic);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_uninterruptible_tasks);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_uninterruptible_tasks_dn);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mmc_blk_reset);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mmc_blk_mq_rw_recovery);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sd_update_bus_speed_mode);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mmc_attach_sd);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sdhci_get_cd);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mmc_gpio_cd_irqt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_save_vmalloc_stack);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_show_stack_hash);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_save_track_hash);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_set_task_comm);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpufreq_acct_update_power);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_typec_tcpm_log);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_media_device_setup_link);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_clear_reserved_fmt_fields);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_fill_ext_fmtdesc);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_clear_mask_adjust);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_v4l2subdev_set_selection);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_v4l2subdev_set_fmt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_v4l2subdev_set_frame_interval);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_scmi_timeout_sync);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_find_new_ilb);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_alloc_uid);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_free_user);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_freq_qos_add_request);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_freq_qos_update_request);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_freq_qos_remove_request);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_set_balance_anon_file_reclaim);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpuidle_psci_enter);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_cpuidle_psci_exit);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sha256);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_aes_expandkey);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_aes_encrypt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_aes_decrypt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_force_compatible_pre);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_force_compatible_post);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_binder_print_transaction_info);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_setscheduler_uclamp);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_rproc_recovery);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ptype_head);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_kfree_skb);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_mmap_file);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_file_open);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_check_bpf_syscall);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_usb_dev_suspend);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_usb_dev_resume);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_ipv6_gen_linklocal_addr);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_sound_usb_support_cpu_suspend);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_snd_compr_use_pause_in_drain);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_modify_thermal_request_freq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_modify_thermal_target_freq);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_enable_thermal_power_throttle);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_thermal_power_cap);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_get_thermal_zone_device);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_typec_tcpm_modify_src_caps);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_try_grab_compound_head);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh___get_user_pages_remote);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_get_user_pages);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_internal_get_user_pages_fast);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_pin_user_pages);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_attach_entity_load_avg);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_detach_entity_load_avg);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_update_load_avg);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_remove_entity_load_avg);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_update_blocked_fair);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_update_rt_rq_load_avg);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_pci_d3_sleep);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_update_rq_clock_pelt);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_rvh_cpumask_any_and_distribute);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_resume_begin);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_resume_end);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_tune_memcg_scan_type);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_pidfd_open);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_mmput);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_early_resume_begin);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_alloc_pages_reclaim_bypass);
EXPORT_TRACEPOINT_SYMBOL_GPL(android_vh_alloc_pages_failure_bypass);
