//
// Copyright (c) 2017, Kentaro Sekimoto
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  -Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//  -Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <stdio.h>
#include <stdint.h>
#include "usbh_hw_dep_include.h"
#include "usbh_env.h"
#include "fwimage469_STA_USB.c"

void set_mac_addr(unsigned char *mac_addr);
void ping_test(void);

#define DEBUG_FUNC
//#define DEBUG_WID
//#define DEBUG_SET_SSID
//#define DEBUG_GET_SSID
//#define DEBUG_MAC_ADDR
//#define DEBUG_MAC_STATUS
//#define DEBUG_PACKET_AVAIL

#define WID_QUERY       0x51
#define WID_WRITE       0x57
#define WID_RESPONSE    0x52
#define WID_INFORMATION 0x49
#define WID_NETWORK     0x4e

#define WID_BSS_TYPE 0x0000         // ALL 通信タイプ R/W 発生
#define WID_CURRENT_CHANNEL 0x0002  // IBSS STA/AP チャネル R/W 発生
#define WID_STATUS 0x0005           // ALL ステータス I －
#define WID_SCAN_TYPE 0x0007        // STA スキャンニング方法 R/W 発生
#define WID_KEY_ID 0x0009           // ALL WEP 暗号化キーインデックス R/W －
#define WID_POWER_MANAGEMENT 0x000B // BSS STA パワーマネージメント機能 R/W 発生
#define WID_11I_MODE 0x000C         // ALL 暗号化方式 R/W 発生
#define WID_AUTH_TYPE 0x000D        // ALL セキュリティ認証タイプ R/W 発生
#define WID_SITE_SURVEY 0x000E      // STA サイトサーベイ(AP/STA スキャン) R/W －
#define WID_LISTEN_INTERVAL 0x000F  // BSS STA リッスンインターバル R/W 発生
#define WID_DTIM_PERIOD 0x0010      // AP DTIM 周期 R/W 発生
#define WID_BCAST_SSID 0x0015       // BSS STA/AP ブロードキャストSSID R/W -/発生
#define WID_DISCONNECT 0x0016       // AP リンク切断 W －
#define WID_REKEY_POLICY 0x0019     // AP RSNA GTK 鍵交換ポリシー R/W 発生
#define WID_START_SCAN_REQ 0x001E   // STA サイトサーベイスキャン開始要求 R/W 発生
#define WID_RSSI 0x001F             // STA RSSI 取得 R －
#define WID_JOIN_REQ 0x0020         // STA 接続要求 W 発生
#define WID_ANTENNA_SELECTION 0x0021    // STA アンテナ選択、ダイバーシティ設定 R/W －
#define WID_CURRENT_MAC_STATUS 0x0031   // STA MAC ステータス取得 R －
#define WID_SCAN_FILTER 0x0036      // STA BSS スキャンのフィルター R/W －
#define WID_DEVICE_READY 0x003D     // ALL デバイス起動通知 I －
#define WID_WPS_START 0x0043        // BSS STA/AP WPS プロトコル開始 R/W －
#define WID_WPS_DEV_MODE 0x0044     // BSS STA WPS デバイスモード R/W －
#define WID_USB_RMTWKUP_TIME 0x0070 // STA Remote Wakeup 信号出力時間 R/W －
#define WID_VSIE_FRAME 0x00B4       // ALL VSIE フレーム選択 R/W －
#define WID_VSIE_INFO_ENABLE 0x00B5 // ALL VSIE 受信通知指定 R/W －
#define WID_DOWNLOAD_REQUEST 0x00F0 // ROM ダウンロードデータ要求 R －
#define WID_START 0x00F1            // ROM ダウンロード完了 R/W －
#define WID_RESET 0x00F2            // ROM ダウンロード開始指示 W －
#define WID_POWER_SAVE 0x0100       // STA スリープ機能 W 発生
#define WID_WAKE_STATUS 0x0101      // STA パワーマネージメント状態通知 I －
#define WID_TX_POWER_RATE 0x0106    // ALL 送信パワー変更 R/W －
#define WID_USB_IN_XFER_MODE 0x0E00 // ALL USB のIN 転送モード R/W －
#define WID_BEACON_INTERVAL 0x1006  // ALL ビーコンインターバル設定 R/W 発生
#define WID_WPS_PASS_ID 0x1017      // AP Device Password ID 設定 R/W －
#define WID_REKEY_PERIOD 0x2010     // AP GTK 更新周期(時間ベース) R/W 発生
#define WID_ENABLE_CHANNEL 0x2024   // ALL 使用チャネル設定 R/W －
#define WID_VSIE_RX_OUI 0x2084      // ALL VSIE 受信フィルタ設定 R/W －
#define WID_SSID 0x3000             // ALL SSID R/W 発生
#define WID_FIRMWARE_VERSION 0x3001 // ALL ファームウェアバージョン R －
#define WID_BSSID 0x3003            // ALL BSSID R －
#define WID_WEP_KEY_VALUE 0x3004    // ALL WEP キー設定 R/W 発生
#define WID_11I_PSK 0x3008          // BSS STA/AP 11i パスフレーズ R/W 発生
#define WID_MAC_ADDR 0x300C         // ALL MAC アドレス取得 R －
#define WID_SITE_SURVEY_RESULTS 0x3012  // STA サイトサーベイ結果取得 R －
#define WID_SERIAL_NUMBER 0x3018    // ALL WLAN モジュール個体識別番号 R －
#define WID_WPS_STATUS 0x3024       // BSS STA/AP WPS プロトコル状態通知 I －
#define WID_WPS_PIN 0x3025          // BSS STA/AP WPS のPIN コード R/W －
#define WID_DOWNLOAD_DATA 0x30F0    // ROM ダウンロードデータ W －
#define WID_WPS_CRED_LIST 0x4006    // BSS STA WPS クレデンシャルリスト R/W －
#define WID_STA_JOIN_INFO 0x4008    // AP 接続情報通知 N －
#define WID_CONNECTED_STA_LIST 0x4009   // AP 接続情報参照 R －
#define WID_VSIE_TX_DATA 0x4085     // ALL VSIE 送信データ R/W －
#define WID_VSIE_RX_DATA 0x4086     // ALL VSIE 受信データ R/I －

#define WID_OK      1
#define WID_ERROR   -1

#define FT_MAC_TX   1
#define FT_MAC_RX   2
#define FT_WID_RX   3
#define FT_WID_TX   4

#define WID_FRAME_MAX   1534
#define WID_DATA_MAX    1530
#define MAC_FRAME_MAX   1534
#define MAC_PAYLOAD_MAX 1520

#define BULKOUT_SIZE        1536
#define BULKIN_SIZE_AFTER   (512+2)

#define WIO_DEF_TIMEOUT 10

// BSS TYPE
#define BSS_TYPE_BSS_STA    0x00
#define BSS_TYPE_IBSS_STA   0x01
#define BSS_TYPE_AP         0x02
#define BSS_TYPE_WLAN_POWER_OFF 0x80

// 11I Mode
#define NO_SEC      0x00
#define WEP64       0x03
#define WEP128      0x07
#define WPA_AES     0x29
#define WPA_TKIP    0x49
#define WPA_TKIP_AES    0x69
#define WPA2_AES    0x31
#define WPA2_TKIP   0x51
#define WPA2_TKIP_AES   0x71
#define WPA_WPA2_MIXED  0x79

#define HUT_WEP64   0x00
#define HUT_TKIP    0x01
#define HUT_AES     0x02
#define HUT_NO_SEC  0x03
#define HUT_WEP128  0x04

typedef struct {
    unsigned char dst[6];
    unsigned char src[6];
    unsigned short type;
    unsigned char data[MAC_PAYLOAD_MAX];
} mac_frame;

typedef struct {
    unsigned char type;
    unsigned char sn;
    unsigned short len;
    unsigned char data[WID_DATA_MAX];
} wid_frame;

typedef struct {
    union {
        unsigned short hdr;
        struct {
            unsigned short len :12;
            unsigned short ft :4;
        };
    };
    union {
        unsigned char frame[WID_FRAME_MAX];
        mac_frame MAC;
        wid_frame WID;
    };
} wio_frame;

static int conn_num;
static st_usb_device *dev;
int g_sn;
wio_frame wio_req;
wio_frame wio_res;
wio_frame wio_packet;
int wio_packet_size;
int avail = 0;
extern st_usbh_env usbh_env;

void wio_set_endpoint(st_usb_device *dev, int type)
{
    switch(type) {
    case FT_MAC_TX:
        dev->info.interface[dev->info.cur_interface].ea_out = 1;
        break;
    case FT_MAC_RX:
        dev->info.interface[dev->info.cur_interface].ea_in = 3;
        break;
    case FT_WID_TX:
        dev->info.interface[dev->info.cur_interface].ea_out = 2;
        break;
    case FT_WID_RX:
        dev->info.interface[dev->info.cur_interface].ea_in = 4;
        break;
    default:
        break;
    }
}

int wio_bulk_out_transfer(int conn_num, st_usb_device *dev, int size, wio_frame* data, int type)
{
    en_usb_status status;

    wio_set_endpoint(dev, type);
    status = ul_bulk_out_transfer(conn_num, dev, size, (unsigned char*)data);
    if (status == USBH_OK)
        return WID_OK;
    else
        return WID_ERROR;
}

int wio_bulk_in_transfer(int conn_num, st_usb_device *dev, int size, wio_frame* data, int type)
{
    en_usb_status status;

    wio_set_endpoint(dev, type);
    status = ul_bulk_in_transfer(conn_num, dev, size, (unsigned char*)data);
    if (status == USBH_OK)
        return WID_OK;
    else
        return WID_ERROR;
}

int wio_reset(int conn_num, st_usb_device *dev, uint32_t *addr, uint16_t *size)
{
    int wid;
    int ret = WID_OK;
    *addr = 0;
    *size = 0;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    wio_req.ft = FT_WID_TX;
    wio_req.len = 8;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 8;
    wio_req.WID.data[0] = (unsigned char)WID_RESET;
    wio_req.WID.data[1] = (unsigned char)(WID_RESET >> 8);
    wio_req.WID.data[2] = 1;
    wio_req.WID.data[3] = 1;
    ret = wio_bulk_out_transfer(conn_num, dev, (int)BULKOUT_SIZE/*size*/, &wio_req, FT_WID_TX);
    if (ret == WID_OK) {
        wio_res.ft = FT_WID_RX;
        wio_res.len = 13;
        ret = wio_bulk_in_transfer(conn_num, dev, (int)wio_res.len + 2/*size*/, &wio_res, FT_WID_RX);
        wid = (uint16_t)wio_res.WID.data[0] + ((uint16_t)wio_res.WID.data[1] << 8);
        *addr = *(uint32_t *)&wio_res.frame[7];
        *size = *(uint16_t *)&wio_res.frame[11];
    } else {
        ret = WID_ERROR;
    }
    g_sn++;
#ifdef DEBUG_WID
    debug_printf(" WID:%04x\r\n", wid);
#endif
    return ret;
}

int wio_downloaddata(int conn_num, st_usb_device *dev, unsigned char *data)
{
    uint32_t addr;
    uint16_t size;
    int ret = WID_ERROR;

    int timeout;
    int i;
    unsigned char sta;
    uint16_t wid = WID_DOWNLOAD_REQUEST;
    unsigned char sumb;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    ret = wio_reset(conn_num, dev, &addr, &size);
    if (ret != WID_OK)
        goto wio_downloaddata_exit;
    while (wid == WID_DOWNLOAD_REQUEST) {
        memset(&wio_req, 0, BULKOUT_SIZE);
        memset(&wio_res, 0, BULKOUT_SIZE);
        sumb = 0;
        for (i = 0; i < size; i++) {
            sumb += data[addr + i];
            wio_req.WID.data[i + 4] = data[addr + i];
        }
        wio_req.WID.data[i + 4] = sumb;
        wio_req.len = size + 9;
        wio_req.ft = FT_WID_TX;
        wio_req.WID.type = WID_WRITE;
        wio_req.WID.sn = g_sn;
        wio_req.WID.len = size + 9;
        wio_req.WID.data[0] = (unsigned char)WID_DOWNLOAD_DATA;
        wio_req.WID.data[1] = (unsigned char)(WID_DOWNLOAD_DATA >> 8);
        wio_req.WID.data[2] = (unsigned char)size;
        wio_req.WID.data[3] = (unsigned char)(size >> 8);
        ret = wio_bulk_out_transfer(conn_num, dev, (int)BULKOUT_SIZE/*size*/, &wio_req, FT_WID_TX);
        if (ret != WID_OK) {
            break;
        }
        udelay(10000);
        wio_res.ft = FT_WID_RX;
        wio_res.len = 13;
        ret = wio_bulk_in_transfer(conn_num, dev, (int)wio_res.len + 2/*size*/, &wio_res, FT_WID_RX);
        if (ret != WID_OK) {
        }
        g_sn++;
        wid = (uint16_t)wio_res.WID.data[0] + ((uint16_t)wio_res.WID.data[1] << 8);
        addr = *(uint32_t *)&wio_res.frame[7];
        size = *(uint16_t *)&wio_res.frame[11];
    }
    sta = wio_res.WID.data[3];
#ifdef DEBUG_WID
    debug_printf(" WID:%04x\r\n", wid);
    debug_printf(" STA:%02x\r\n", sta);
#endif
    if (!((wid == WID_START) && (sta == 1))) {
#ifdef DEBUG_WID_START
        debug_printf(" WID_START received.\r\n");
#endif
        goto wio_downloaddata_exit;
    }
    memset(&wio_req, 0, BULKOUT_SIZE);
    memset(&wio_res, 0, BULKOUT_SIZE);
    wio_req.len = 8;
    wio_req.ft = FT_WID_TX;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 8;
    wio_req.WID.data[0] = (unsigned char)WID_START;
    wio_req.WID.data[1] = (unsigned char)(WID_START >> 8);
    wio_req.WID.data[2] = 1;
    wio_req.WID.data[3] = 1;
    ret = wio_bulk_out_transfer(conn_num, dev, (int)BULKOUT_SIZE, &wio_req, FT_WID_TX);
    if (ret != WID_OK) {
        goto wio_downloaddata_exit;
    }
    wio_res.ft = FT_WID_RX;
    wio_res.len = 8;
    ret = wio_bulk_in_transfer(conn_num, dev, (int)BULKIN_SIZE_AFTER, &wio_res, FT_WID_RX);
    if (ret != WID_OK) {
    }
    wid = (uint16_t)wio_res.WID.data[0] + ((uint16_t)wio_res.WID.data[1] << 8);
    sta = wio_res.WID.data[3];
    if (wid == WID_DEVICE_READY) {
        ret = WID_OK;
        goto wio_downloaddata_exit;
    }
    g_sn++;
#ifdef DEBUG_WID
    debug_printf(" WID_START \r\n");
    debug_printf(" WID:%04x\r\n", wid);
    debug_printf(" STA:%02x\r\n", sta);
#endif
    udelay(1000000);
    wio_req.len = 8;
    wio_req.ft = FT_WID_TX;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 8;
    wio_req.WID.data[0] = (unsigned char)WID_START;
    wio_req.WID.data[1] = (unsigned char)(WID_START >> 8);
    wio_req.WID.data[2] = 1;
    wio_req.WID.data[3] = 1;
    ret = wio_bulk_out_transfer(conn_num, dev, (int)BULKOUT_SIZE, &wio_req, FT_WID_TX);
    if (ret != WID_OK) {
        goto wio_downloaddata_exit;
    }
    wio_res.ft = FT_WID_RX;
    wio_res.len = 8;
    ret = wio_bulk_in_transfer(conn_num, dev, (int)BULKIN_SIZE_AFTER, &wio_res, FT_WID_RX);
    if (ret != WID_OK) {
    }
    wid = (uint16_t)wio_res.WID.data[0] + ((uint16_t)wio_res.WID.data[1] << 8);
    sta = wio_res.WID.data[3];
    g_sn++;
#ifdef DEBUG_WID
    debug_printf(" WID_START \r\n");
    debug_printf(" WID:%04x\r\n", wid);
    debug_printf(" STA:%02x\r\n", sta);
#endif
    udelay(1000000);
    wio_req.len = 8;
    wio_req.ft = FT_WID_TX;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 8;
    wio_req.WID.data[0] = (unsigned char)WID_START;
    wio_req.WID.data[1] = (unsigned char)(WID_START >> 8);
    wio_req.WID.data[2] = 1;
    wio_req.WID.data[3] = 1;
    ret = wio_bulk_out_transfer(conn_num, dev, (int)BULKOUT_SIZE, &wio_req, FT_WID_TX);
    if (ret != WID_OK) {
        ret = WID_ERROR;
        goto wio_downloaddata_exit;
    }
    ret = wio_bulk_in_transfer(conn_num, dev, (int)BULKIN_SIZE_AFTER, &wio_res, FT_WID_RX);
    if (ret != WID_OK) {
    }
    wid = (uint16_t)wio_res.WID.data[0] + ((uint16_t)wio_res.WID.data[1] << 8);
    sta = wio_res.WID.data[3];
    g_sn++;
    wio_downloaddata_exit:
#ifdef DEBUG_WID
    debug_printf(" WID_START \r\n");
    debug_printf(" WID:%04x\r\n", wid);
    debug_printf(" STA:%02x\r\n", sta);
#endif
    return ret;
}

int wio_wid_bulk_transfer(int conn_num, st_usb_device *dev, wio_frame* wio_req, wio_frame* wio_res)
{
    int ret = WID_ERROR;
    ret = wio_bulk_out_transfer(conn_num, dev, (int)BULKOUT_SIZE, wio_req, FT_WID_TX);
    if (ret == WID_OK) {
        ret = wio_bulk_in_transfer(conn_num, dev, (int)BULKIN_SIZE_AFTER, wio_res, FT_WID_RX);
    }
    g_sn++;
    return ret;
}

int wio_wid_bulk_transfer_wait_ok(int conn_num, st_usb_device *dev, wio_frame* wio_req, wio_frame* wio_res, int timeout)
{
    int wid;
    unsigned char sta;
    int ret = WID_ERROR;
    while (timeout-- > 0) {
        ret = wio_wid_bulk_transfer(conn_num, dev, wio_req, wio_res);
        wid = (uint16_t)(wio_res->WID.data[0]) + ((uint16_t)(wio_res->WID.data[1]) << 8);
        sta = wio_res->WID.data[3];
#ifdef DEBUG_WID
        debug_printf(" WID:%04x\r\n", wid);
        debug_printf(" STA:%02x\r\n", sta);
#endif
        if (sta == 1)
            break;
        udelay(1000000);
    }
    return ret;
}

int wio_wid_bulk_transfer_wid(int conn_num, st_usb_device *dev, wio_frame* wio_req, wio_frame* wio_res, int timeout, unsigned short wid)
{
    int _wid;
    int ret = WID_ERROR;
    while (timeout-- > 0) {
        ret = wio_wid_bulk_transfer(conn_num, dev, wio_req, wio_res);
        _wid = (uint16_t)(wio_res->WID.data[0]) + ((uint16_t)(wio_res->WID.data[1]) << 8);
#ifdef DEBUG_WID
        debug_printf(" WID:%04x\r\n", _wid);
#endif
        if (_wid == wid)
            break;
        udelay(1000000);
    }
    return ret;
}

// unsigned char bss_type
// BSS_TYPE_BSS_STA    0x00
// BSS_TYPE_IBSS_STA   0x01
// BSS_TYPE_AP         0x02
// BSS_TYPE_WLAN_POWER_OFF 0x80
int wio_set_bss_type(int conn_num, st_usb_device *dev, unsigned char bss_type)
{
    int ret;
#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    wio_req.ft = FT_WID_TX;
    wio_req.len = 8;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 8;
    wio_req.WID.data[0] = (unsigned char)WID_BSS_TYPE;
    wio_req.WID.data[1] = (unsigned char)(WID_BSS_TYPE >> 8);
    wio_req.WID.data[2] = 1;
    wio_req.WID.data[3] = bss_type;
    ret = wio_wid_bulk_transfer_wait_ok(conn_num, dev, &wio_req, &wio_res, WIO_DEF_TIMEOUT);
    return ret;
}

int wio_set_ssid(int conn_num, st_usb_device *dev, unsigned char *ssid)
{
    int ret;
    int i;
    unsigned char len;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    len = (unsigned char)strlen(ssid);
    wio_req.ft = FT_WID_TX;
    wio_req.len = len + 7;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = (unsigned short)(len + 7);
    wio_req.WID.data[0] = (unsigned char)WID_SSID;
    wio_req.WID.data[1] = (unsigned char)(WID_SSID >> 8);
    wio_req.WID.data[2] = len;
#ifdef DEBUG_SET_SSID
    debug_printf(" SSID:");
#endif
    for (i = 0; i < len; i++) {
        wio_req.WID.data[i + 3] = *ssid;
#ifdef DEBUG_SET_SSID
        debug_printf("%c", *ssid);
#endif
        ssid++;
    }
#ifdef DEBUG_SET_SSID
    debug_printf("\r\n");
#endif
    ret = wio_wid_bulk_transfer_wait_ok(conn_num, dev, &wio_req, &wio_res, WIO_DEF_TIMEOUT);
    return ret;
}

int wio_get_ssid(int conn_num, st_usb_device *dev, unsigned char *ssid, unsigned char size)
{
    int ret;
    int i;
    unsigned char len;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    wio_req.ft = FT_WID_TX;
    wio_req.len = 6;
    wio_req.WID.type = WID_QUERY;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 6;
    wio_req.WID.data[0] = (unsigned char)WID_SSID;
    wio_req.WID.data[1] = (unsigned char)(WID_SSID >> 8);
    ret = wio_wid_bulk_transfer_wid(conn_num, dev, &wio_req, &wio_res, WIO_DEF_TIMEOUT, WID_SSID);
#ifdef DEBUG_GET_SSID
    debug_printf(" SSID:");
#endif
    len = wio_res.WID.data[2];
    if (len > size)
        len = size;
    for (i = 0; i < len; i++) {
        *ssid = wio_res.WID.data[i + 3];
#ifdef DEBUG_GET_SSID
        debug_printf("%c", *ssid);
#endif
        ssid++;
    }
    *ssid = 0;
#ifdef DEBUG_GET_SSID
    debug_printf("\r\n");
#endif
    return ret;
}

int wio_set_11i_psk(int conn_num, st_usb_device *dev, unsigned char *key)
{
    int ret;
    int i;
    unsigned char len;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    len = (unsigned char)strlen(key);
    wio_req.ft = FT_WID_TX;
    wio_req.len = len + 7;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = (unsigned short)(len + 7);
    wio_req.WID.data[0] = (unsigned char)WID_11I_PSK;
    wio_req.WID.data[1] = (unsigned char)(WID_11I_PSK >> 8);
    wio_req.WID.data[2] = len;
    for (i = 0; i < len; i++) {
        wio_req.WID.data[i + 3] = key[i];
    }
    ret = wio_wid_bulk_transfer_wait_ok(conn_num, dev, &wio_req, &wio_res, WIO_DEF_TIMEOUT);
    return ret;
}

int wio_set_11i_mode(int conn_num, st_usb_device *dev, unsigned char mode)
{
    int ret;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    wio_req.ft = FT_WID_TX;
    wio_req.len = 8;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 8;
    wio_req.WID.data[0] = (unsigned char)WID_11I_MODE;
    wio_req.WID.data[1] = (unsigned char)(WID_11I_MODE >> 8);
    wio_req.WID.data[2] = 1;
    wio_req.WID.data[3] = mode;
    ret = wio_wid_bulk_transfer_wait_ok(conn_num, dev, &wio_req, &wio_res, WIO_DEF_TIMEOUT);
    return ret;
}

int wio_set_wep_key_value(int conn_num, st_usb_device *dev, unsigned char *key)
{
    int ret;
    int i;
    unsigned char len;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    len = (unsigned char)strlen(key);
    wio_req.ft = FT_WID_TX;
    wio_req.len = len + 7;
    wio_req.WID.type = WID_WRITE;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = (unsigned short)len;
    wio_req.WID.data[0] = (unsigned char)WID_WEP_KEY_VALUE;
    wio_req.WID.data[1] = (unsigned char)(WID_WEP_KEY_VALUE >> 8);
    wio_req.WID.data[2] = len;
    for (i = 0; i < len; i++) {
        wio_req.WID.data[i + 3] = key[i];
    }
    ret = wio_wid_bulk_transfer_wait_ok(conn_num, dev, &wio_req, &wio_res, WIO_DEF_TIMEOUT);
    return ret;
}

int wio_get_mac_addr(int conn_num, st_usb_device *dev, unsigned char *addr)
{
    int i;
    int ret;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    wio_req.ft = FT_WID_TX;
    wio_req.len = 6;
    wio_req.WID.type = WID_QUERY;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 6;
    wio_req.WID.data[0] = (unsigned char)WID_MAC_ADDR;
    wio_req.WID.data[1] = (unsigned char)(WID_MAC_ADDR >> 8);
    ret = wio_wid_bulk_transfer_wid(conn_num, dev, &wio_req, &wio_res, WIO_DEF_TIMEOUT, WID_MAC_ADDR);
#ifdef DEBUG_MAC_ADDR
    debug_printf(" MAC ADDR:");
#endif
    for (i = 0; i < 6; i++) {
        addr[i] = wio_res.WID.data[3 + i];
#ifdef DEBUG_MAC_ADDR
        debug_printf("%02x", addr[i]);
#endif
    }
#ifdef DEBUG_MAC_ADDR
    debug_printf("\r\n");
#endif
    return ret;
}

int wio_get_current_mac_status(int conn_num, st_usb_device *dev, unsigned char *status)
{
    int i;
    int ret;

#ifdef DEBUG_FUNC
    debug_printf("%s\r\n", __func__);
#endif
    wio_req.ft = FT_WID_TX;
    wio_req.len = 6;
    wio_req.WID.type = WID_QUERY;
    wio_req.WID.sn = g_sn;
    wio_req.WID.len = 6;
    wio_req.WID.data[0] = (unsigned char)WID_CURRENT_MAC_STATUS;
    wio_req.WID.data[1] = (unsigned char)(WID_CURRENT_MAC_STATUS >> 8);
    ret = wio_wid_bulk_transfer_wid(conn_num, dev, &wio_req, &wio_res, WIO_DEF_TIMEOUT, WID_CURRENT_MAC_STATUS);
    if (ret == WID_OK)
        *status = wio_res.WID.data[3];
    else
        *status = -1;
#ifdef DEBUG_MAC_STATUS
    debug_printf(" MAC STATUS:%d\r\n", *status);
#endif
    return ret;
}

int wio_tx(int conn_num, st_usb_device *dev, unsigned char *data, unsigned short size)
{
    return WID_OK;
}

int wio_rx(int conn_num, st_usb_device *dev, unsigned char *data, unsigned short size)
{
    return WID_OK;
}

void Packet_Send(unsigned char *packet, int size)
{
    int ret;
    wio_req.ft = FT_MAC_TX;
    wio_req.len = (unsigned short)size; // WID_FRAME_MAX
    memcpy((void *)&wio_req.MAC, (void *)packet, size);
    ret = wio_bulk_out_transfer(conn_num, dev, (int)BULKOUT_SIZE, &wio_req, FT_MAC_TX);
}

int Packet_Receive(unsigned char *packet, int size)
{
    int ret;
    if (avail) {
        size = wio_packet_size;
        memcpy((void *)packet, (void *)&wio_packet.MAC, size);
        avail = 0;
    } else
        size = 0;
    return size;
}

int Packet_Avail(void)
{
    int ret;
    if (avail == 0) {
        ret = wio_bulk_in_transfer(conn_num, dev, (int)BULKIN_SIZE_AFTER, &wio_res, FT_MAC_RX);
        if (ret == WID_OK) {
            if (wio_res.len != 0) {
                wio_packet_size = wio_res.len;
                memcpy((void *)&wio_packet, (void *)&wio_res, wio_res.len + 2);
                avail = 1;
#ifdef DEBUG_PACKET_AVAIL
                debug_printf("%s\r\n", __func__);
                debug_printf(" Size:%d\r\n", wio_res.len);
#endif
                return 1;
            }
        }
    }
    return 0;
}

#define MAX_SSID_SIZE   64
unsigned char ssid[] = "xxxxxxxxxx\0";
unsigned char key[] = "xxxxxxxxxx\0";
unsigned char addr[7];
unsigned char buf[MAX_SSID_SIZE];

int wlan_init(void)
{
    unsigned char bss_type = BSS_TYPE_BSS_STA;
    unsigned char mode = WEP64;
    unsigned char status;

    conn_num = 0;
    dev = usbh_env.conn[conn_num].p_root;

    wio_downloaddata(conn_num, dev, (unsigned char*)&g_firmware_image);
    wio_set_bss_type(conn_num, dev, bss_type);
    wio_set_ssid(conn_num, dev, ssid);
    wio_set_11i_psk(conn_num, dev, key);
    wio_set_11i_mode(conn_num, dev, mode);
    wio_set_wep_key_value(conn_num, dev, key);
    //wio_get_ssid(conn_num, dev, &buf, MAX_SSID_SIZE);
    wio_get_mac_addr(conn_num, dev, addr);
    wio_get_current_mac_status(conn_num, dev, &status);
    set_mac_addr(addr);
}

int main()
{
    avail = 0;
    g_sn = 1;
    uh_init();
    udelay(200000);
    wlan_init();
    udelay(1000000);
    ping_test();
    return 0;
}
