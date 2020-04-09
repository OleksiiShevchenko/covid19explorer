
void updateScreen (String country, String cases, String deaths, String recovered) {
  lcd.clear();
  lcd.print(country);
  lcd.setCursor(0, 1);
  lcd.print(cases + "/");
  lcd.print(deaths + "/");
  lcd.print(recovered);
}



void updateCountryScreen (String country) {
  lcd.clear();
  lcd.print(country);
  lcd.setCursor(0, 1);
  lcd.print("Loading...");
}


int countryIndexCounter (int index, bool increase) {
  
  int countriesMaxIndex = sizeof(countryList)/sizeof(countryList[0]) - 1;
  
  if (increase) {
    index += 1;
  } else {
    index -= 1;
  }
  
  if (index < 0) index = countriesMaxIndex;
  if (index > countriesMaxIndex) index = 0;
  Serial.println("dataIndex");
  Serial.print(index);
  return index;
}
