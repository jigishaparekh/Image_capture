#include "esp_camera.h"
#include "camera_pins.h"

#define button_press  5// configure button pin

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(button_press, INPUT);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  // config.frame_size = FRAMESIZE_UXGA;
  config.frame_size = FRAMESIZE_HD; //can change frame size as per requirement
  config.pixel_format = PIXFORMAT_JPEG;  // Use JPEG format for image data
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 10;  //12
  config.fb_count = 1;

  // Camera initialization
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  s->set_awb_gain(s, 1);     // Enable AWB Gain  // 1 = Enable, 0 = Disable
  s->set_aec_value(s, 100);  // Enable AEC -> Adjust exposure value (0 - 1200)
  s->set_gain_ctrl(s, 1);    // Enable gain control
  s->set_lenc(s, 1);         // Enable or disable Lens Correction
  // Optional: Adjust camera settings (e.g., brightness, saturation)
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // Flip image vertically
    s->set_brightness(s, 1);   // Increase brightness
    s->set_saturation(s, -2);  // Decrease saturation
  }
  Serial.println("Camera initialized.");
}


void image_capture() {
  // Capture an image
  camera_fb_t *fb = esp_camera_fb_get(); //frame buffr to store captured image in hex format
  Serial.printf("Image size: %d bytes\n", fb->len);  //print the size of image
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  // Print image data in HEX format
  Serial.println("Captured Image Data (HEX):");
  for (size_t i = 0; i < fb->len; i++) {
    Serial.printf("%02X", fb->buf[i]);  // Print each byte in HEX
    if ((i + 1) % 32 == 0) {            // Add a newline every 32 bytes
      Serial.println();
    }
  }
  Serial.println("\n\n");
  Serial.printf("\nResolution: %dx%d\n", fb->width, fb->height);
  // Return the frame buffer back to be reused
  esp_camera_fb_return(fb);
  Serial.printf("\nimage captured successfully");
  // delay(20000);  // Capture an image every 20 seconds
}

void loop() {
    if (!digitalRead(button_press)) {  // Button press detected (active LOW)
    Serial.println("Button pressed! Capturing image...");
    image_capture();
    delay(50);  // Debounce delay to avoid multiple triggers
  }
}