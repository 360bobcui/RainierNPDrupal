<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
function site_adminstration_form($form, $form_state) {
  $form = array();  
  $header = array(
    'camp_site_id' => t('Camp Site ID'),
    'fullname' => t('Name'),
    'capacity' => t('Capacity'),
  );
  
  $sql = db_select('site_info', 'si')->extend('PagerDefault');

  $sql->fields('si', array('id','fullname','capacity'))
      ->orderBy('id','AESC');
  $results = $sql->execute();
  
  $rows = array();
  dsm($results);
  foreach ($results as $result) {
    
    // shall match the keys in headers
    $rows[] = array(
      'camp_site_id' => $result->id,
      'fullname' => $result->fullname,
      'capacity' => $result->capacity,
      '#attributes' => array('class' => array('site-info-row')),
    );
  }
  
//    $form['flag_application_status']['approvedeny'] = array(
//      '#type' => 'select',
//      '#title' => 'Actions',
//      '#options' => array(
//         1 => t('Approve'),
//         2 => t('Deny'),
//      ),
//    );
  
    $form['site_info_table'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#empty' => t('No content available.'),
      '#attributes' => array('class' => array('site_info_table')),
    );
  
    $form['pager'] = array("#markup" => theme('pager'));
      
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('admin_return'),
    );
  return $form; 
}

function admin_return($form, &$form_state) {
  drupal_goto('site_reservation');
  
}
//
//// process submitted form
//function flag_application_form_submit($form, &$form_state) {
////  $action = $form_state['values']['approvedeny'];
////  $values = array();
////  foreach($form_state['values']['flag_application_table'] as $keys => $value) {
////    if (is_string($value) == TRUE) {
////      $values[] = $form_state['complete form']['flag_application_table']['#options'][$value];
////    }
////  }
////  flag_application_administration($action, $values);
//}
//
//function flag_application_form_validate($form, &$form_state) {
////  dsm($form);
////  dsm($form_state);
////  if ($form_state['values']['approvedeny'] != 1 && $form_state['values']['approvedeny'] != 2) {
////    form_set_error('approvedeny', "You have proved an incorrect value");
////  }
//  
//}
//
//
//function flag_application_administration($action, $values) {
////  $status = ($action == 1) ? 'Approved' : 'Denied'; //text is goingto show in Reports/Recent log message
////  foreach ($values as $key => $value) {
////    $num_updated = db_update('flag_application')
////        ->fields(array('status' => $action))
////        ->condition('flagging_id', $value['flagging_id'])
////        ->execute();
////    if (module_exists('rules')) {
////      rules_invoke_event('flag_application_reviewed', $value['flagging_id']. $action);
////    }
////    watchdog('flag_application', t("!name's application for !node was !status", 
////        array('name' => $value['name'], '!node' => $value['title'], '!status' => $status)));
////    
////  }
//}
