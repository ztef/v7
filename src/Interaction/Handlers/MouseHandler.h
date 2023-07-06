#pragma once
#include <iostream>
#include <vsg/all.h>

class MouseHandler : public vsg::Inherit<vsg::Visitor, MouseHandler>
{
public:
    vsg::ref_ptr<vsg::Trackball> trackball0;
    vsg::ref_ptr<vsg::Trackball> trackball1;
    int trackball_active;

    // Constructor : Recibe un builder para poder crear objetos en demanda.

    MouseHandler(vsg::ref_ptr<vsg::Trackball> t0, vsg::ref_ptr<vsg::Trackball> t1) : trackball0(t0),
                                                                                     trackball1(t1)
    {
    }


     

    // Aplica a eventos de teclado .
    /*

    void apply(vsg::KeyPressEvent& keyPress) override
    {

    }


    // Si el boton 1 del mouse es presionado, sensa por intersecciones :

    void apply(vsg::ButtonPressEvent& buttonPressEvent) override
    {

    }
    */


    void apply(vsg::KeyPressEvent &evt)
      {
          if(trackball_active == 0){
              trackball0->apply(evt);

          } else {
               trackball1->apply(evt);
          }

      }

        void apply(vsg::ButtonPressEvent &evt)
      {
          if(trackball_active == 0){
              trackball0->apply(evt);

          } else {
               trackball1->apply(evt);
          }

      }

    void apply(vsg::ButtonReleaseEvent &evt)
      {
          if(trackball_active == 0){
              trackball0->apply(evt);

          } else {
               trackball1->apply(evt);
          }

      }

    
    
      void apply(vsg::MoveEvent &moveEvent)
      {
          if(trackball_active == 0){
              trackball0->apply(moveEvent);

          } else {
               trackball1->apply(moveEvent);
          }

      }

       void apply(vsg::ScrollWheelEvent &move)
      {
          if(trackball_active == 0){
              trackball0->apply(move);

          } else {
               trackball1->apply(move);
          }

      }

       void apply(vsg::TouchDownEvent &move)
      {
          if(trackball_active == 0){
              trackball0->apply(move);

          } else {
               trackball1->apply(move);
          }

      }

       void apply(vsg::TouchUpEvent &move)
      {
          if(trackball_active == 0){
              trackball0->apply(move);

          } else {
               trackball1->apply(move);
          }

      }

       void apply(vsg::TouchMoveEvent &move)
      {
          if(trackball_active == 0){
              trackball0->apply(move);

          } else {
               trackball1->apply(move);
          }

      }


     void apply(vsg::FrameEvent &move)
      {
          if(trackball_active == 0){
              trackball0->apply(move);

          } else {
               trackball1->apply(move);
          }

      }



   
};