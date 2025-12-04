#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(wifi_auto, LOG_LEVEL_INF);

// #define WIFI_SSID "ZeahoGuestWIFI"       // 修改为你的 Wi-Fi 名称
// #define WIFI_PASS "zeaho2021"   // 修改为你的 Wi-Fi 密码
#define WIFI_SSID "tmkwifi"       // 修改为你的 Wi-Fi 名称
#define WIFI_PASS "12345678"   // 修改为你的 Wi-Fi 密码

int main(void)
{
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = sizeof(WIFI_SSID) - 1,
        .psk = WIFI_PASS,
        .psk_length = sizeof(WIFI_PASS) - 1,
        .security = WIFI_SECURITY_TYPE_PSK,
        .channel = WIFI_CHANNEL_ANY,
    };

    LOG_INF("Starting Wi-Fi auto reconnect demo");

    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface,
                       &wifi_params, sizeof(wifi_params));
    if (ret) {
        LOG_ERR("Failed to initiate Wi-Fi connection: %d", ret);
    }

    while (1) {
        struct wifi_iface_status status = { 0 };
        int ret;

        // 获取 WiFi 接口状态
        ret = net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, iface,
                      &status, sizeof(struct wifi_iface_status));

        if (ret == 0) {
            LOG_INF("WiFi Status - State: %d, SSID: %s, RSSI: %d dBm, Channel: %d",
                   status.state, status.ssid, status.rssi, status.channel);

            // 打印连接状态的文字描述
            if (status.state == WIFI_STATE_COMPLETED) {
                printk("WiFi: CONNECTED to %s (RSSI: %d dBm)\n",
                      status.ssid, status.rssi);
            } else if (status.state == WIFI_STATE_DISCONNECTED) {
                printk("WiFi: DISCONNECTED\n");
            } else if (status.state == WIFI_STATE_SCANNING) {
                printk("WiFi: SCANNING\n");
            } else if (status.state == WIFI_STATE_ASSOCIATING) {
                printk("WiFi: ASSOCIATING\n");
            } else if (status.state == WIFI_STATE_ASSOCIATED) {
                printk("WiFi: ASSOCIATED\n");
            } else if (status.state == WIFI_STATE_4WAY_HANDSHAKE) {
                printk("WiFi: 4-WAY HANDSHAKE\n");
            } else {
                printk("WiFi: UNKNOWN STATE (%d)\n", status.state);
            }
        } else {
            LOG_ERR("Failed to get WiFi status: %d", ret);
            printk("WiFi: Status unavailable\n");
        }

        printk("Uptime: %d ms\n", k_uptime_get_32());

        k_sleep(K_SECONDS(5));
    }
}
