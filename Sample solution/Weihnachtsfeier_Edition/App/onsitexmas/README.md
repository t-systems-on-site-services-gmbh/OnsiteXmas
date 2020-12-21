# OnsiteXmas

[![npm version](https://badge.fury.io/js/ionic.svg)](https://badge.fury.io/js/ionic)

Einfach Dokumentation zu der neuen OnsiteXmas App Version 1.0 (release)

# New Features!

  - Neue UI/UX Oberfläche
  - BLE mit spezifischer Suche
  - Verschiedene Auswahlmöglichkeiten:
    - Farbprofil (RGB Colorpicker und Vordefinierte Profile)
    - Heiligkeit
  -  Bug Fixes

### Tech

OnsiteXmas verwendet eine Reihe von Open Source-Projekten, um ordnungsgemäß zu funktionieren:

* [Ionic] - UI toolkit for mobile and desktop apps 
* [Capacitor] - Capacitor is a cross-platform native runtime
* [Cordova] - Target multiple platforms with one code base
* [AngularJS] - HTML enhanced for web apps!
* [node.js] - evented I/O for the backend
* [jQuery] - duh

### Plugins

OnsiteXmas wird derzeit um folgende Plugins erweitert. Die Dokumentation finden Sie in den jeweiligen Beschreibung unten: 

| Plugin | Documentation |
| ------ | ------ |
| cordova-plugin-screen-orientation | [Documentation][cpso] |
| cordova-plugin-ble-central | [Documentation][cpbc] |
| ngx-color-picker | [Documentation][ncp] |

### Installation

OnsiteXmas ist Node basierend. Installiere die dependencies und devDependencies um danach mit `ionic serve` um die App im Browser zu start. 

```sh
$ cd onsitexmas
$ npm install -d
$ ionic serve
```

Wenn dass nicht funtkioniert dann, ...

```sh
$ npm install -g @ionic/cli
```

### Development

Nach der Installation kann man direkt weiterentwickeln. Falls man einen Applikation daraus entwickelkn will man folgendes bei den jeweiligen Anbietern tun. 

#### Ionic Build und Capacitor

Erster Schritt - Erzeugen eines `www` - Ordner :
```sh
$ ionic build
```

Zweiter Schritt - Erzeugen von `android` und `ios` Ordner  (Note: beide **müssen** hinzugefügt sein):
```sh
$ npx cap add ios
$ npx cap add android
```

Dritter Schritt - Custom App Icon und Splashscreen (orientert an dem [Artikel]):

```sh
$ npm install cordova-res --save-dev
$ npm run resources
```

bei Änderrung des Codes danach folgende Reihenfolge beachten:

```sh
$ ionic build
$ npx cap sync
```

#### Für Android

Entweder man ruft das Projekt mit `npx cap open android` direkt auf. Falls das nicht funktioniert öffnet man in Android Studios manuell den `android` Ordner. Danach Build > Build Apk

Weiteres Vorgehen wird in der Dokumentation von Capacitor erklären: [Deploying your Capacitor Android App to the Google Play Store]

#### Für IOS

Das gleiche wie bei Android, entweder mit `npx cap open ios` direkt auf. Falls das nicht funktioniert öffnet man in Xcode manuell den `ios` Ordner.

Weiteres Vorgehen wird in der Dokumentation von Capacitor erklären: [Deploying your Capacitor iOS App to the App Store]

### Ideen zur Weitentwicklung

 - Mehrer Geräte gleichzeitig bedienen 
 - Einrichten von Farbanimationen
 - usw. (Orientieren an Philips Hue bzw. das weiterentwickeln)

#### Leute, die dran gearbeitet haben (bei Weiterentwicklung, bitte dazu schreiben)
 - Version 0.0.1: Philip Thanh Vu Tran, philip-thanh-vu.tran@t-systems.com
 - Version 1.0: Denis Kreiner, denis.kreiner@t-systems.com


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [Ionic]: <https://ionicframework.com/docs/>
   [Capacitor]: <https://capacitorjs.com/>
   [Cordova]: <https://cordova.apache.org//>
   [AngularJS]: <http://angularjs.org>
   [node.js]: <http://nodejs.org>
   [jQuery]: <http://jquery.com>
   [cpso]: <https://cordova.apache.org/docs/en/latest/reference/cordova-plugin-screen-orientation/>
   [cpbc]: <https://github.com/don/cordova-plugin-ble-central>
   [ncp]: <https://market.ionicframework.com/themes/custome-color-picker>
   [Artikel]: <https://medium.com/@dalezak/generate-app-icon-and-splash-screen-images-for-ionic-framework-using-capacitor-e1f8c6ef0fd4>
   [Deploying your Capacitor iOS App to the App Store]: <https://capacitorjs.com/docs/ios/deploying-to-app-store>
   [Deploying your Capacitor Android App to the Google Play Store]: <https://capacitorjs.com/docs/android/deploying-to-google-play>
