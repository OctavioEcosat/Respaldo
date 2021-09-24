<?php
    include ('connection.php');

    $query = "SELECT * FROM sensor_doors";
    $result = mysqli_query($conn, $query);

    if(!$result){
        die('Query Error: ' . mysqli_error($conn));
    }
    //Content-type application/x-www-form-urlencoded

    $json = array();
    while($row = mysqli_fetch_assoc($result)){
        $json[] = array(
            'id' => $row['id'],
            'data' => $row['data']
        );
    }

    $jsonstring = json_encode($json);
    echo $jsonstring;