
#include "algorithm.h"
#include "policy.h"
#include "policy_fifo.h"
#include "policy_random.h"


int vmem_get_policy_descriptor(vmem_algorithm_t al, vmem_policy_descriptor_t* pout_policy) {
    switch (al) {
        case VMEM_RANDOM:
            pout_policy->initialize     = vmem_policy_random_initialize;
            pout_policy->on_hit         = vmem_policy_random_on_hit;
            pout_policy->on_miss        = vmem_policy_random_on_miss;
            pout_policy->select_victim  = vmem_policy_random_select_victim;
            pout_policy->destroy        = vmem_policy_random_destroy;

            return 0;
        case VMEM_FIFO:
            pout_policy->initialize     = vmem_policy_fifo_initialize;
            pout_policy->on_hit         = vmem_policy_fifo_on_hit;
            pout_policy->on_miss        = vmem_policy_fifo_on_miss;
            pout_policy->select_victim  = vmem_policy_fifo_select_victim;
            pout_policy->destroy        = vmem_policy_fifo_destroy;

            return 0;
        case VMEM_LRU:
        case VMEM_OPTIMAL:
        case VMEM_IDEAL:
        case VMEM_INVALID:
            return 1;
        default: break;
    }

    return -1;
}
