<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function site_submitted_itinerary_form() {
  $form = array();
   $form['home'] = array(
    '#type' => 'submit',
    '#name' => 'home',
    '#value' => t('Return'),
    '#submit' => array('site_submitted_itinerary_form_submit_handler')
  );  
  return $form;
}

function site_submitted_itinerary_form_submit_handler(){
  drupal_goto('site_reservation');  
}