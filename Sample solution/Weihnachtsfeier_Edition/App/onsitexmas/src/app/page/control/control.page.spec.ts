import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { ControlPage } from './control.page';

describe('ControlPage', () => {
  let component: ControlPage;
  let fixture: ComponentFixture<ControlPage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ControlPage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(ControlPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
