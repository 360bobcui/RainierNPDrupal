<?php
/* 
 * Approve and disapprove reservation page. Administrator permission only
 */
function site_adminstration_form($form, $form_state) {
  $form = array();  
  $header = array(
    'id' => t('ID'),
//    'name' => t('User Name'),
    'uid' => t('User ID'),
    'itinerary' => t('Itinerary'), // a list of camp sites, a camper going to stay
    'period' => t('Period'), // period of the reservation covers
    'time' => t('Date Sumbitted'), // date the reservation submitted
    'status' => t('Status'),    
  );
  
  $sql = db_select('site_pending_itinerary', 'si')->extend('PagerDefault');
  $sql->fields('si', array('id','uid','itinerary','period','date_stamp', 'status'))
      ->orderBy('date_stamp','AESC');
  $results = $sql->execute();
  
  $rows = array();
  //dsm($results);
  foreach ($results as $result) {
    switch ($result->status){
      case 0:
        $status = 'Pending';
        break;
      case 1:
        $status = 'Approved';
        break;
      case 2:
        $status = 'Disapproved';
        break;      
    }
    
    $rows[] = array(  // keys should match the headers above
      'id' => $result->id,
      'uid' => $result->uid,
      'itinerary' => $result->itinerary,
      'period' => $result->period,
      'time' => $result->date_stamp,
       'status' => $status,
      '#attributes' => array('class' => array('site-info-row')),
    );
  }
  
    $form['site_reservation_status']['approvedeny'] = array(
      '#type' => 'select',
      '#title' => 'Actions',
      '#options' => array(
         1 => t('Approve'),
         2 => t('Deny'),
      ),
    );
  
    $form['site_reservation_table'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#empty' => t('No content available.'),
      '#attributes' => array('class' => array('reservations')),
    );
  
    $form['pager'] = array("#markup" => theme('pager'));
      
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  return $form; 
}

// process submitted form
function site_adminstration_form_submit($form, &$form_state) {
  $action = $form_state['values']['approvedeny'];
  $values = array();
  //dsm($form_state);
  foreach($form_state['values']['site_reservation_table'] as $keys => $value) {
    if (is_string($value) == TRUE) {
      $values[] = $form_state['complete form']['site_reservation_table']['#options'][$value];
    }
  }
  site_reservation_administration($action, $values);  // update site_permit database and watchdog
}

function site_adminstration_form_validate($form, &$form_state) {
//  dsm($form);
//  dsm($form_state);
  if ($form_state['values']['approvedeny'] != 1 && $form_state['values']['approvedeny'] != 2) {
    form_set_error('approvedeny', "You have proved an incorrect value");
  }
  
}

function getArrivalDateFromPeriod($period) {
   $ddArr = explode(' ', $period);
   $start_date = $ddArr[0];
   //$ad_date = mktime(0, 0, 0, $adArr[1], $adArr[2], $adArr[0]);  
   return $start_date;
  }
  
  // given the start date and the increment, return start + increment
  function getDateToString ($base_date, $increment) {
    $base_dateArr = explode('-', $base_date); // 2016-08-18       
    $ad_date = mktime(0, 0, 0, $base_dateArr[1], $base_dateArr[2] + $increment, $base_dateArr[0]); 
    return date('Y-m-d', $ad_date);
  }
  
  // Called by site_reservation_administration(). 
  //  verify if $site_id and $uid are in the database
//function isInDataBase($db, $site_id, $uid) {
//  $res = db_select($db, 'db')
//      ->fields('db',array('site_id', 'uid'))
//      ->condition('site_id', $site_id)
//      ->condition('uid', $uid)
//      ->execute();   
//  dsm($res);
//  return count($res);
//}

function site_reservation_administration($action, $values) {
  $status = ($action == 1) ? 'Approved' : 'Denied'; //text is goingto show in Reports/Recent log message
  //$values store all the data in extracted from db
  global $user;
  foreach ($values as $key => $value) {
    $num_updated = db_update('site_pending_itinerary')
        ->fields(array('status' => $action))
        ->condition('id', $value['id'])
        ->execute();
    
    // issue permits for each day in the itinerary
    if($action == 1) {  // insert row in site_permits table.  Permits issued
      $arrival_date = getArrivalDateFromPeriod($value['period']); 
      $site_ids = explode(' ', $value['itinerary']);  // site ids
      $i = 0;
      foreach($site_ids as $site_id) {
        $permit_date = getDateToString ($arrival_date,  $i); 
        $res = db_select('site_permits', 'sp')
            ->fields('sp',array('site_id', 'uid', 'date'))
            ->condition('site_id', $site_id)
            ->condition('uid', $user->uid)
            ->condition('date', $permit_date)
            ->execute();
        $num_exsiting_entries = 0;
        foreach($res as $re) {
          $num_exsiting_entries++;
        }
        dsm($num_exsiting_entries);        
          //dsm($res);
        if ($num_exsiting_entries == 0){
          $res = db_insert('site_permits')
              ->fields(array(
             'uid' => $user->uid,
             'version' => time(),
             'date' => $permit_date,
              'site_id'=> $site_id,    
            ))
             
          ->execute();
        }
        $i++;
      }
    } 
    
//    if (module_exists('rules')) {
//      rules_invoke_event('flag_application_reviewed', $value['flagging_id']. $action);
//    }
   // dsm($value);
    watchdog('site_reservation', t("!name's reservation for !node was !status", 
        array('uid' => $value['uid'], '!id' => $value['id'], '!status' => $status)));
//    
  }
  
  //get the period of the itinerary selected from db, return the start date
  
}
