#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Narasimhan H");
MODULE_DESCRIPTION("Real-Time IDS with Attack Detection and Safe Traffic Labeling");

#define LOG_PREFIX "IDS"
#define DNS_PORT 53

struct ip_stat {
    __be32 ip;
    int syn_count, null_count, xmas_count;
    int udp_count, icmp_count, dns_count;
    struct hlist_node hnode;
};

DEFINE_HASHTABLE(ip_table, 10);

static struct nf_hook_ops nfho;

static unsigned int hook_func(void *priv,
    struct sk_buff *skb,
    const struct nf_hook_state *state)
{
    struct iphdr *iph;
    struct tcphdr *tcph;
    struct udphdr *udph;
    struct icmphdr *icmph;
    struct ip_stat *entry;
    __be32 src_ip;
    unsigned int dst_port = 0;
    char verdict[64] = "Safe Traffic";
    bool suspicious = false;

    if (!skb)
        return NF_ACCEPT;

    iph = ip_hdr(skb);
    if (!iph)
        return NF_ACCEPT;

    src_ip = iph->saddr;

    // Get or create entry for this IP
    hash_for_each_possible(ip_table, entry, hnode, src_ip) {
        if (entry->ip == src_ip)
            break;
        entry = NULL;
    }

    if (!entry) {
        entry = kmalloc(sizeof(*entry), GFP_KERNEL);
        if (!entry) return NF_ACCEPT;
        entry->ip = src_ip;
        entry->syn_count = entry->null_count = entry->xmas_count = 0;
        entry->udp_count = entry->icmp_count = entry->dns_count = 0;
        hash_add(ip_table, &entry->hnode, src_ip);
    }

    switch (iph->protocol) {
        case IPPROTO_TCP:
            tcph = (struct tcphdr *)((__u32 *)iph + iph->ihl);
            dst_port = ntohs(tcph->dest);

            if (tcph->syn && !tcph->ack && !tcph->fin && !tcph->rst && !tcph->psh && !tcph->urg) {
                entry->syn_count++;
                if (entry->syn_count > 100) {
                    snprintf(verdict, sizeof(verdict), "SYN Flood Detected");
                    suspicious = true;
                }
            } else if (!(tcph->syn || tcph->ack || tcph->fin || tcph->rst || tcph->psh || tcph->urg)) {
                entry->null_count++;
                if (entry->null_count > 30) {
                    snprintf(verdict, sizeof(verdict), "NULL Scan Detected");
                    suspicious = true;
                }
            } else if (tcph->fin && tcph->psh && tcph->urg) {
                entry->xmas_count++;
                if (entry->xmas_count > 30) {
                    snprintf(verdict, sizeof(verdict), "XMAS Scan Detected");
                    suspicious = true;
                }
            }
            break;

        case IPPROTO_UDP:
            udph = (struct udphdr *)((__u32 *)iph + iph->ihl);
            dst_port = ntohs(udph->dest);

            if (dst_port == DNS_PORT) {
                entry->dns_count++;
                if (entry->dns_count > 30) {
                    snprintf(verdict, sizeof(verdict), "DNS Tunneling: Possible");
                    suspicious = true;
                } else {
                    snprintf(verdict, sizeof(verdict), "DNS Query: Safe");
                }
            } else {
                entry->udp_count++;
                if (entry->udp_count > 100) {
                    snprintf(verdict, sizeof(verdict), "UDP Flood Detected");
                    suspicious = true;
                }
            }
            break;

        case IPPROTO_ICMP:
            icmph = (struct icmphdr *)((__u32 *)iph + iph->ihl);
            entry->icmp_count++;
            if (entry->icmp_count > 100) {
                snprintf(verdict, sizeof(verdict), "ICMP Flood Detected");
                suspicious = true;
            }
            break;

        default:
            snprintf(verdict, sizeof(verdict), "Unknown Protocol: Safe");
            break;
    }

    // Print log
    char src_ip_str[16];
    snprintf(src_ip_str, 16, "%pI4", &iph->saddr);

    printk(KERN_INFO "[%s] Proto=%d Src=%s DstPort=%u Verdict=%s\n",
           LOG_PREFIX, iph->protocol, src_ip_str, dst_port, verdict);

    return NF_ACCEPT;
}

static int __init ids_init(void)
{
    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;
    nf_register_net_hook(&init_net, &nfho);
    printk(KERN_INFO "[%s] Module loaded\n", LOG_PREFIX);
    return 0;
}

static void __exit ids_exit(void)
{
    struct ip_stat *entry;
    struct hlist_node *tmp;
    int bkt;

    nf_unregister_net_hook(&init_net, &nfho);
    hash_for_each_safe(ip_table, bkt, tmp, entry, hnode) {
        hash_del(&entry->hnode);
        kfree(entry);
    }
    printk(KERN_INFO "[%s] Module unloaded\n", LOG_PREFIX);
}

module_init(ids_init);
module_exit(ids_exit);
