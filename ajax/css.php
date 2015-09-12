<?php
/*

Simple JSON interface with autoload functionality.

*/

function simpleAutoLoad($class) {
  require_once './../includes/'.strtolower($class).'.inc';
}


spl_autoload_register('simpleAutoLoad');


switch ($_GET['op']) {
case 'get':
  css_get();
  break;
case 'insert':
  css_insert();
  break;
}

function css_get() {
  $postdata = file_get_contents("php://input");
  $request = json_decode($postdata, true);

  $results = array();

  $record_css = new DataCss();

  if (isset($request['record_id'])) {
    foreach ($request['record_id'] as $record_id) {
      $record_css->getById($record_id);
      $results = array_merge($results, $record_css->response());
    }
  }else { // all
    $record_css->getAll();
    $results = $record_css->response();
  }

  $response = array(
    'success' => 1,
    'records' => $results
  );
  echo json_encode($response);
}


function css_insert() {
  $postdata = file_get_contents("php://input");
  $request = json_decode($postdata, true);
  $record_css = new InsertCss($request['filename'], $request['filedata']);

  if ($record_css->success()) {
    $response = array(
      'success' => 1,
      'records' => $record_css->response()
    );
  } else {
    $response = array(
      'success' => 0,
      'error' => $record_css->error()
    );
  }
  echo json_encode($response);
}