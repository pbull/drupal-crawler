<?php

/**
 * Crawl homepage content.
 */

$crawl_list['<front>'] = array(
  'type' => 'front',
  'alias' => FALSE,
  'url' => url('<front>', array('alias' => TRUE, 'absolute' => TRUE)),
);

$front_page = variable_get('site_frontpage', 'node');
$crawl_list[$path] = array(
  'type' => 'front',
  'alias' => FALSE,
  'url' => url($front_page, array('alias' => TRUE, 'absolute' => TRUE)),
);

// Path alias
if ($alias = drupal_lookup_path('alias', $front_page)) {
  $crawl_list[$alias] = array(
    'type' => 'front',
    'alias' => TRUE,
    'url' => url($front_page, array('absolute' => TRUE)),
  );
}
