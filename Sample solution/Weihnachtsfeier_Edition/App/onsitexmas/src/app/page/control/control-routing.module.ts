import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { ControlPage } from './control.page';

const routes: Routes = [
  {
    path: '',
    component: ControlPage,
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class ControlPageRoutingModule {}
