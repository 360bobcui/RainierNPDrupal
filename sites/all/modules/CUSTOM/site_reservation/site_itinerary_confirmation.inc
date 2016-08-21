<?php

/* 
 * view the submission of the user
 */

function site_submitted_itinerary_form() {
  $form = array();
  
   $form['home'] = array(
    '#prefix' => '<div class="confirmation-form"> Your reservation has been '
     . 'successfully submitted for review. You should receive an email momentarily '
     . 'for instructions to'
     . 'complete the payment. Once the payment is received, you will receive another email'
     . 'to confirm your reservation within in three business days.',
    '#suffix' => '</div>', 
  );  
   
   $form['return_home'] = array(
    '#type' => 'submit',
    '#name' => 'home',
    '#value' => t('Check Reservation Submitted'),
    '#submit' => array('site_submitted_itinerary_form_submit_handler'),
   );
  return $form;
}


//  TODO: add a page to view all the submissions
function site_submitted_itinerary_form_submit_handler(){
  drupal_goto('site_reservation');  
}