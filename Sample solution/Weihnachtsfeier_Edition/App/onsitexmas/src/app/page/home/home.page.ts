import { Component } from '@angular/core';
import { Router } from '@angular/router';
import { ToastController } from '@ionic/angular';


@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage{
  timer: any; 
  preventSimpleClick: boolean;
  preventSimpleCheck: boolean;

  constructor(
    private router : Router,
    private toastCtrl: ToastController
  ) {
  }

  //Easter Egg
  async singleClick(){
    this.timer = 0;
    this.preventSimpleClick = false;
    let delay = 200;

    this.timer = setTimeout(async () => {
      if(!this.preventSimpleClick){
        let toast = this.toastCtrl.create({
          message: 'Design made by Freshman Project ',
          duration: 3000,
          color: 'primary',
          position: 'bottom'
        });
        (await toast).present();
        console.log("simple click");
      }
    }, delay);

  }

  doubleClick(): void{
    this.preventSimpleCheck = true;
    clearTimeout(this.timer);
    window.location.href='https://www.instagram.com/freshman_project/';
    console.log("double click");
  }

  navigateToScan(){
    this.router.navigate(['scan']);
  }
}
