
String getData(String country) {
  
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  
  HTTPClient https;
  
  https.begin(*client, requestUrl + country);
  int httpCode = https.GET();
  String payload = https.getString();
  
  Serial.println(httpCode);
  Serial.println(payload);

  https.end();

  return payload;
}
