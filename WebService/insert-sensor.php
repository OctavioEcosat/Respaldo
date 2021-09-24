<?php
    include './php/connection.php';

    $id = $_GET['id'];
    $data = $_GET['data'];

    echo "$id $data <br>";
    
    $query = "CALL SP_LOGS(1, $id, $data)";
    $result = mysqli_query($conn, $query);

    if(!$result){
        die('Query Error: ' . mysqli_error($conn));
    }

?>