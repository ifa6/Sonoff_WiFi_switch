void initUpgrade() {
  // Добавляем функцию Update для перезаписи прошивки по Wi-Fi при 1М(256K SPIFFS) и выше
  httpUpdater.setup(&HTTP);
  HTTP.on("/upgrade", webUpgrade);                // запустить обновление
  modulesReg("upgrade");
}
// ----------------------- Обновление с сайта
void webUpgrade() {
  String refresh = "<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"./css/build.css\"><meta http-equiv=\"refresh\" content=\"60;\"></head><body><br><br><center><div class=\"loader\"></div><h1>Update module...</h1></center></body></html>";
  //httpOkText(refresh);
    HTTP.send(200, "text/html", refresh);
  String spiffsData = HTTP.arg(spiffsS);
  if (spiffsData != "") {
    //SPIFFS.format();
    spiffsData = spiffsData.substring(spiffsData.lastIndexOf("/") + 1); // выделяем имя файла
    ESPhttpUpdate.rebootOnUpdate(false);
    t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs(HTTP.arg(spiffsS));
switch(ret) {
            case HTTP_UPDATE_FAILED:
            jsonWrite(configSetup, spiffsDataS, "Spiffs_FAILED");
            statistics();
            break;
            case HTTP_UPDATE_NO_UPDATES:
            jsonWrite(configSetup, spiffsDataS, "Spiffs_NO_UPDATES");
            statistics();
                break;
            case HTTP_UPDATE_OK:
            jsonWrite(configSetup, spiffsDataS, "Spiffs_UPDATE_OK");
            statistics();
                break;
        }
    writeFile("timer.save.json", jsonTimer);
    writeFile("scenary.save.txt", Scenary);
    jsonWrite(configSetup, spiffsDataS, spiffsData);
    saveConfigSetup ();
  }
  String buildData = HTTP.arg("build");
  if (buildData != "") {
    buildData = buildData.substring(buildData.lastIndexOf("/") + 1); // выделяем имя файла
    jsonWrite(configSetup, buildDataS, buildData);
    saveConfigSetup ();
    ESPhttpUpdate.rebootOnUpdate(true);
    t_httpUpdate_return jet = ESPhttpUpdate.update(HTTP.arg("build"));

  }
}


