import { Component } from '@angular/core';

import { AlertController, Platform } from '@ionic/angular';
import { SplashScreen } from '@ionic-native/splash-screen/ngx';
import { ScreenOrientation } from '@ionic-native/screen-orientation/ngx';
import { StatusBar } from '@ionic-native/status-bar/ngx';

import { Location } from '@angular/common';
import { BLE } from '@ionic-native/ble/ngx';
import { Router } from '@angular/router';

@Component({
  selector: 'app-root',
  templateUrl: 'app.component.html',
  styleUrls: ['app.component.scss']
})
export class AppComponent {

  peripheral: any = {};

  constructor(
    private platform: Platform,
    private splashScreen: SplashScreen,
    private statusBar: StatusBar,

    private alertController: AlertController,
    private _location: Location,
    private screenOrientation: ScreenOrientation,
    private ble: BLE,
    private router: Router
  ) 
  {
    this.initializeApp();
  }

  initializeApp() {
    this.platform.ready().then(() => {
      this.statusBar.styleDefault();
      this.splashScreen.hide();
      this.screenOrientation.lock(this.screenOrientation.ORIENTATIONS.PORTRAIT);
  });

  
    /** 
  * * Its bugged, will be investigated by Ionic during Milestone 5.5.1 https://github.com/ionic-team/ionic-framework/milestone/96
  */
  this.platform.backButton.subscribeWithPriority(10, (processNextHandler) => {
    console.log('Back press handler!');
    if (this._location.isCurrentPathEqualTo('/home')) {
      // Show Exit Alert!
      console.log('Show Exit Alert!');
      this.showExitConfirm();
      processNextHandler();
    } else if(this._location.isCurrentPathEqualTo('/scan') ){
      this.router.navigate(['home']);
    } else {
      // Navigate to back page
      console.log('Navigate to back page');
      this._location.back();
    }
  });

  this.platform.backButton.subscribeWithPriority(5, () => {
    console.log('Handler called to force close!');
    this.alertController.getTop().then(r => {
      if (r) {
          navigator['app'].exitApp();
        }
      }).catch(e => {
        console.log(e);
      })
    });
  }

  showExitConfirm() {
    this.alertController.create({
      header: 'App termination',
      message: 'Do you want to close the app?',
      backdropDismiss: false,
      buttons: [{
        text: 'Stay',
        role: 'cancel',
        handler: () => {
          console.log('Application exit prevented!');
        }
      }, {
        text: 'Exit',
        handler: () => {
          navigator['app'].exitApp();
        }
      }]
    })
    .then(alert => {
      alert.present();
    });
  }
}
