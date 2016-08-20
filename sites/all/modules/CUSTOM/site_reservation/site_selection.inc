<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * allow users to select the camp sites
 */
function site_selection_form() {
//  $options = drupal_get_query_parameters();
  $options = drupal_get_query_parameters();
  //dsm($options);

  $form = array();
//  $form['locations[']['text'] = array(
//    '#markup' => '<span> '.$options['arrival_date']. '</span> ',
//  );
  
  $arrival_date = date_create($options['arrival_date']); // yyyy-mm-dd format
  $departure_date = date_create($options['departure_date']);
 // arrival_date_departure_date_validation($arrival_date, $departure_date, NULL, NULL);
  
  $interval = date_diff($arrival_date, $departure_date);
   
  $form['locations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Itinerary'),
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
    '#prefix' => '<div class="poll-form"> Please select camp site(s)',
    '#suffix' => '</div>', 

  );
  
  $table_headers = array(t('Date'), t('Camp Site'));
  $rows = array();
//  foreach ($results AS $result) {
//    $rows[] = array(
//        $result->id,
//        $result->fullname,
//      );    
//  }
//  theme('table', array('header' => $table_headers, 'rows' => $rows));
  
  for($i = 0; $i < $interval->format('%a') + 1; $i++) {  // first day inclusive
    $this_dt = getDateToString($options['arrival_date'], $i);  // return the yyyy-mm-dd based   
    $sites = getOptions($this_dt);  
    $form["locations"]['location'.$i] = array(
        '#type' => 'select',
        '#title' => $this_dt,
        '#title_display' => 'after',
        '#options' => $sites,
         //'#markup' => '<span> '.$options['arrival_date']. '</span> ',
    ); 
//    $rows[] = array(
//     // drupal_render($this_dt),
//      drupal_render($form["locations"]['location'.$i]),  
//      drupal_render($form["locations"]['location'.$i]),      
//    );
  }
 //theme('table', array('header' => $table_headers, 'rows' => $rows));
 //theme('table', $table_headers, $rows, array('style' => 'width: 100%', 'class' => 'form-item'));
 $form['camp_select'] = array(
    '#type' => 'submit',
    '#name' => 'camp_select_btn',
    '#value' => t('Reserve'),
    '#submit' => array('site_selection_submit_handler')
  );  
  //$form['#submit'] = 'site_selection_form_submit';
  return $form;
}


/*
 * given the base_date and the increment, return date = base_date + increment, in 
 *  yyyy-mm-dd format
 */
function getDateToString ($base_date, $increment) {
    $base_dateArr = explode('-', $base_date); // 2016-08-18       
    $ad_date = mktime(0, 0, 0, $base_dateArr[1], $base_dateArr[2] + $increment, $base_dateArr[0]); 
    return date('Y-m-d', $ad_date);
}

/*
 * get the list of camp sites available given the date
 * TO DO: join with permits table to check any permits issued in the given date
 */
function getOptions($this_date) {
  //$sql = db_select('site_info', 'si')->extend('PagerDefault');
  $sql = db_select('site_info', 'si');
  $sql->fields('si', array('id','fullname'))
      ->condition('capacity',0, '>'); // capacity > 0
      //->orderBy('s','AESC');
//  $results = db_query('SELECT si.id, si.fullname
//    FROM {site_info} WHERE si.capacity > 0');
  $results = $sql->execute();
  //dsm($results);
  $rows = array();
  $i = 0;
  foreach($results AS $result) {
    $rows[$i] = $result->id . ' '. $result->fullname;    
    $i++;    
  };
  //dsm($i);
 return $rows;    
//  
//  $headers = array(t('ID'), t('Full name'), t('Capacity'));
//  $rows = array();
//  foreach ($results AS $result) {
//    $rows[] = array(
//        $result->id,
//        $result->fullname,
//        $result->capacity,   
//      );    
//  }
//  return theme('table', array('header' => $headers, 'rows' => $rows));
  
}

function site_selection_submit_handler($form, &$form_state) {
//  dsm($form_state);
//   dsm($form);
   $option = drupal_get_query_parameters();
  $options = array(
    'at' => 1,
    'att' => 2,
  );
  $itinerary = '';
  $keys = array_keys($form_state['values']); // get all the keys
//  dsm($keys);
  $i = 0;
  foreach($keys as $key) {
    //dsm($key);
    if (strpos($key, "location") !== false){ // key contains location
     // print_r($key);
      $itinerary .= ($form_state['values'][$key]. ' ');      
    }
    $i++;
  }
  
  $period =  $option['arrival_date'].' '.$option['departure_date'];
  global $user;
  $nid = db_insert('site_pending_itinerary')
      ->fields(array(
         'uid' => $user->uid,
         'itinerary' => $itinerary,
         'period' => $period,
        ))
      ->execute();    
  
  
  
  drupal_goto('site_submitted_itinerary');
  //drupal_goto('site_submitted_itinerary', $options);
}

//function site_submitted_itinerary_form() {
//  $form = array();
//  return $form;
//}
