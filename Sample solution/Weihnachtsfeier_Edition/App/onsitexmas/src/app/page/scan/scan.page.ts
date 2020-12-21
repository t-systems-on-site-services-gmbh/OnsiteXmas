import { Component, NgZone } from '@angular/core';
import { NavigationExtras, Router } from '@angular/router';
import { BLE } from '@ionic-native/ble/ngx';
import { LoadingController, ToastController } from '@ionic/angular';

@Component({
  selector: 'app-scan',
  templateUrl: './scan.page.html',
  styleUrls: ['./scan.page.scss'],
})
export class ScanPage {

  devices: any[] = [];
  statusMessage: string;
  peripheral: any = {};

  constructor(
    private loadingController: LoadingController, 
    private ble:BLE, 
    private ngZone: NgZone, 
    private toastCtrl: ToastController, 
    private router : Router
  ) { 

  }

  ionViewWillEnter(){
    this.scan();
    this.presentLoading();
  }

  ionViewWillLeave(){
    this.ble.stopScan();
  }

  async presentLoading() {
    const loading = await this.loadingController.create({
      message: 'Please wait...',
      duration: 1000
    });
    await loading.present();
  }

  scan() {
    this.devices = [];
    this.ble.scan([], 5).subscribe(
      device => this.onDeviceDiscovered(device)
    );
  }

  onDeviceDiscovered(device) {
    console.log("onDeviceDiscovered pop")
    console.log('Discovered' + JSON.stringify(device,null,2));
    this.ngZone.run(()=>{
      this.devices.push(device)
      console.log(device)
    })
  }

  openDetailsWithState(p) {
    let navigationExtras: NavigationExtras = {
      state: {
        user: p
      }
    };
    this.router.navigate(['control'], navigationExtras);
  }
  
  Connect(device) {
    this.ble.stopScan();
    this.ble.connect(device.id).subscribe(
      peripheral => this.onConnected(peripheral),
      peripheral => this.onDeviceDisconnected(peripheral)
    );
  }

  async onConnected(peripheral) {
    let toast = this.toastCtrl.create({
      message: 'Connected to ' + peripheral.name,
      duration: 3000,
      position: 'bottom'
    });
    (await toast).present();
    this.ngZone.run(() => {
      this.setStatus('');
      this.peripheral = peripheral;
    });
    this.openDetailsWithState(peripheral);
  }

  async onDeviceDisconnected(peripheral) {
    let toast = this.toastCtrl.create({
      message: 'Disconnected from ' + peripheral.name,
      duration: 3000,
      position: 'bottom'
    });
    (await toast).present();
    this.router.navigate(['scan']);
  }
  
  setStatus(message) {
    console.log(message);
    this.ngZone.run(() => {
      this.statusMessage = message;
    });
  }

  stringToBytes(string) {
    var array = new Uint8Array(string.length);
    for (var i = 0, l = string.length; i < l; i++) {
        array[i] = string.charCodeAt(i);
    }
    return array.buffer;
  }
}
