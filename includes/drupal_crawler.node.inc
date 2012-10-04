<?php

/**
 * Crawl node content.
 */

$node_counter = 0;
$rows = db_select('node', 'n')
  ->fields('n',array('nid'))
  ->condition('n.status', '0', '!=')
  ->condition('n.changed', $timestamp_changed, '>=')
  ->orderBy('n.changed', 'DESC')
  ->execute();
  
if ($rows) {
  foreach ($rows as $row) {
    
    $path = 'node/' . $row->nid;
    $alias = drupal_lookup_path('alias', $path);
    $alias_only = ($alias && drush_get_option('alias-only')) ? TRUE : FALSE;
    
    // Path node/NID
    if (!$alias_only) {
      $crawl_list[$path] = array(
        'type' => 'node',
        'nid' => $row->nid,
        'alias' => FALSE,
        'url' => url('node/' . $row->nid, array('alias' => TRUE, 'absolute' => TRUE)),
      );
    }
    
    // Path alias
    if ($alias) {
      $crawl_list[$alias] = array(
        'type' => 'node',
        'nid' => $row->nid,
        'alias' => TRUE,
        'url' => url('node/' . $row->nid, array('absolute' => TRUE)),
      );
    }
    
    
    
    /*
    // Path node/NID
    $path = 'node/' . $row->nid;
    $crawl_list[$path] = array(
      'type' => 'node',
      'nid' => $row->nid,
      'alias' => FALSE,
      'url' => url('node/' . $row->nid, array('alias' => TRUE, 'absolute' => TRUE)),
    );
    
    // Path alias
    //$path_language = 'en';
    //if ($alias = drupal_lookup_path('alias', $path, $path_language)) {
    if ($alias = drupal_lookup_path('alias', $path)) {
      $crawl_list[$alias] = array(
        'type' => 'node',
        'nid' => $row->nid,
        'alias' => TRUE,
        'url' => url('node/' . $row->nid, array('absolute' => TRUE)),
      );
    }
    */
    
    $node_counter++;
  }
}
//drush_print(dt('Crawled @count nodes.', array('@count' => $node_counter)));
