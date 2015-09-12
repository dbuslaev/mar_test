<?php
/*

Core Database class, responsible for retrieving and inserting data.

*/

class CreativeMongo {

  const CODE_SUCCESS = 1;
  const CODE_ERROR_INSERT = -1;
  const CODE_ERROR_FIND = -2;

  const DB_NAME = 'creative_market';
  const TABLE_ID = '_id';

  protected $status_code;
  protected $response;


  protected function find($params = array()) {
    $this->response = array();

    $mongo_client = new MongoClient();
    $collection = $mongo_client->selectCollection(static::DB_NAME, static::TABLE_NAME);

    $records = $collection->find($params);
    foreach ($records as $record) {
      $this->response[] = $record;
    }
  }


  protected function insert($record) {
    $this->response = array();

    $mongo_client = new MongoClient();
    $collection = $mongo_client->selectCollection(static::DB_NAME, static::TABLE_NAME);

    $record[static::TABLE_TIME] = new MongoDate(time());

    $collection->insert($record);
    if ($record[self::TABLE_ID]) {
      $this->response[] = $record;
      $this->status_code = self::CODE_SUCCESS;
    }else $this->status_code = self::CODE_ERROR_INSERT;
  }


  public function response() {
    return $this->response;
  }


  public function success() {
    return $this->status_code == self::CODE_SUCCESS ? true : false;
  }


  protected static function printError($status_code) {
    $errors = array(
      self::CODE_SUCCESS => "",
      self::CODE_ERROR_INSERT => "Database insert error",
      self::CODE_ERROR_FIND => "Database find error"
    );
    return $errors[$status_code];
  }


  public function error() {
    return static::printError($this->status_code);
  }


}