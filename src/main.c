#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <string.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/debug/stack.h>

#include <zephyr/display/mb_display.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

/*
  - 動作
	- 1秒ごとにカウンターが＋１され、printk で出力される
    - セントラルと接続時
		- セントラルからREADされると、カウンターの値を返す。
		- セントラルからWRITEされると、カウンターの値を更新する
*/
uint32_t count = 0;

//----------------------------------------------------------------------
//サービスとキャラクタリスティクスのUUID
/** @brief Service UUID of Example Custom Service **/
// 1605b267-ca22-4f4e-b7ea-0955ad54ca00
#define BT_UUID_EXAMPLE_SERVICE_VAL \
	BT_UUID_128_ENCODE(0x1605b267, 0xca22, 0x4f4e, 0xb7ea, 0x0955ad54ca00)

/** @brief Characteristic UUID of EXAMPLE - Read/Write **/
// ebbff71b-8a65-4596-be37-d845e2e7667e
#define BT_UUID_EXAMPLE_CHAR_VAL \
	BT_UUID_128_ENCODE(0xebbff71b, 0x8a65, 0x4596, 0xbe37, 0xd845e2e7667e)

#define BT_UUID_EXAMPLE_SERVICE	BT_UUID_DECLARE_128(BT_UUID_EXAMPLE_SERVICE_VAL)
#define BT_UUID_EXAMPLE_CHAR	BT_UUID_DECLARE_128(BT_UUID_EXAMPLE_CHAR_VAL)
#define EXAMPLE_CHAR_NAME	"ExampleChar"
//セントラルからREADされたときに呼ばれる関数
static ssize_t read_EXAMPLE(struct bt_conn* conn,
	const struct bt_gatt_attr* attr,
	void* buf,
	uint16_t len,
	uint16_t offset)
{
	uint8_t buffer[4];
	uint16_t buffer_len;
	printk("%s\n", __FUNCTION__);

	buffer_len = 4;
	buffer[0] = (uint8_t)(count >> 24);
	buffer[1] = (uint8_t)(count >> 16);
	buffer[2] = (uint8_t)(count >> 8);
	buffer[3] = (uint8_t)count;
	return bt_gatt_attr_read(conn, attr, buf, len, offset,
		buffer,
		buffer_len);
}
//セントラルからWRITEされたときに呼ばれる関数
static ssize_t write_EXAMPLE(struct bt_conn* conn,
	const struct bt_gatt_attr* attr,
	const void* buf,
	uint16_t len,
	uint16_t offset,
	uint8_t flags)
{
	const char* const buffer = buf;
	ARG_UNUSED(conn);	//avoid warning
	printk("%s\n", __FUNCTION__);
	uint32_t value = 0;
	for (int i = 0; i < len; i++)
	{
		value = (value << 8) + buffer[i];
		printk("%02x ", buffer[i]);
	}
	printk("\n");
	count = value;

	return len;
}

//こんな感じで定義する
/* EXAMPLE BLE Service Declaration */
BT_GATT_SERVICE_DEFINE(EXAMPLE_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_EXAMPLE_SERVICE),
	// Read/Write
	BT_GATT_CHARACTERISTIC(BT_UUID_EXAMPLE_CHAR,
		BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
		BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
		read_EXAMPLE, write_EXAMPLE, NULL),
	BT_GATT_CUD(EXAMPLE_CHAR_NAME, BT_GATT_PERM_READ),
);

//----------------------------------------------------------------------
//セントラルと接続したら呼ばれる関数
static void connected(struct bt_conn* conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err %u)\n", err);
		return;
	}
	printk("Connected\n");
}
//セントラルから切断したら呼ばれる関数
static void disconnected(struct bt_conn* conn, uint8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);
}
//こんな感じで設定する
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

//----------------------------------------------------------------------
// アドバタイズ
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_EXAMPLE_SERVICE_VAL),
};
static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_EXAMPLE_SERVICE_VAL),
};

//----------------------------------------------------------------------
int main(void)
{
	int err;
	err = bt_enable(NULL);
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	while (1)
	{
		printk("%08x\n", count++);
		k_msleep(1000);
	}
	return 0;
}
