<?php
/*

Keeps track of temporary storage of data within file system.
If you have more than one request to the same record within 1 second, it will retrieve it from file and not from database.
This is a quick caching solution without any additional software needed.

*/

class Recordset extends CreativeMongo {

  // consider local file data to be usable for these many seconds
  const CSS_PATH = '/mnt/tmp/';
  const LOCAL_DATA_RELEVANT = 1;


  protected function checkJsonTime($saved_time) {
    return ($saved_time >= time() - static::LOCAL_DATA_RELEVANT) ? true : false;
  }


  protected function localJsonFilenameAll() {
    return static::CSS_PATH.static::TABLE_NAME.'.json';
  }


  protected function localJsonUnlinkAll() {
    $local_filename = $this->localJsonFilenameAll();
    unlink($local_filename);
  }


  protected function localJsonFilenameById($record_id) {
    return static::CSS_PATH.static::TABLE_NAME.'_'.$record_id.'.json';
  }


  public function getAll() {
    $local_filename = $this->localJsonFilenameAll();

    $loaded_locally = $this->readLocallyJson($local_filename);
    if (!$loaded_locally) {
      $this->find();

      $this->saveLocallyJson($local_filename);
    }
  }


  public function getById($record_id) {
    $record_id = new MongoId($record_id);
    $local_filename = $this->localJsonFilenameById((string)$record_id);

    $loaded_locally = $this->readLocallyJson($local_filename);
    if (!$loaded_locally) {
      $request = array(
        static::TABLE_ID => $record_id
      );
      $this->find($request);

      $this->saveLocallyJson($local_filename);
    }
  }


  protected function saveLocallyJson($local_filename) {
    $saved_data = json_encode(
      array(
        'time' => time(),
        'records' => $this->response
      )
    );
    file_put_contents($local_filename, $saved_data);
  }


  protected function readLocallyJson($local_filename) {
    $loaded_locally = false;
    if (file_exists($local_filename)) {
      $saved_data = json_decode(file_get_contents($local_filename), true);

      if ($this->checkJsonTime($saved_data['time'])) {
        $this->response = $saved_data['records'];
        $loaded_locally = true;
      }
    }
    return $loaded_locally;
  }


}