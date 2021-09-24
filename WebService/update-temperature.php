<?php

    include './php/connection.php';

    function validate($val){
        if (empty($val)) {
            return 0;
        }
        else {
            return $val;
        }
    }

    $id = validate($_GET['id']);
    $data = validate($_GET['data']);

    echo "$id $data <br>";

    $query = "UPDATE sensor_activities SET valor = $data where id = $id";
    $result = mysqli_query($conn, $query);

    if(!$result){
        die('Query Error: ' . mysqli_error($conn));
    }

?>