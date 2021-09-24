<?php
    include ('connection.php');
    
    $query =    "SELECT sa.id id, sa.valor valor, 
                    CASE WHEN sa.valor > s.maxLevel THEN 'Temperatura Alta'
                        WHEN sa.valor < s.minLevel THEN 'Temperatura Baja'
                        ELSE 'Temperatura Normal'
                    END statusTemperatura
                FROM sensor_activities sa INNER JOIN sensors s ON s.id = sa.idSensor";
    $result = mysqli_query($conn, $query);

    if(!$result){
        die('Query Error: ' . mysqli_error($conn));
    }
    //Content-type application/x-www-form-urlencoded
    $json = array();
    while($row = mysqli_fetch_assoc($result)){
        $json[] = array(
            'id' => $row['id'],
            'valor' => $row['valor'],
            'statusTemperatura' => $row['statusTemperatura']
        );
    }

    $jsonstring = json_encode($json);
    echo $jsonstring;

?>
