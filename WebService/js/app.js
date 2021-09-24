$(document).ready(function(){
        setInterval(getSensorData, 5000);
        setInterval(getDoorData, 500);
});

function getSensorData(){ 
    $.ajax({
        type: "POST",
        url: "./php/consultaSensores.php",
        success:  function(response){
            let values = JSON.parse(response);
            
            values.forEach(value => {
                $('#sensor_values'+value.id).text(Number.parseFloat(value.valor).toFixed(1) +" °C");
            });
        }
    });
}

function getDoorData(){
    $.ajax({
        type: "POST",
        url: "./php/consultaPuertas.php",
        success:  function(response){
            let values = JSON.parse(response);

            values.forEach(value => {
                if (value.data == 1) {
                    $("#door_values"+value.id).css('color', 'red');
                }
                else{
                    $("#door_values"+value.id).css('color', 'green');
                }
                let data = value.data == 1 ? "ABIERTO": "CERRADO";
                $("#door_values"+value.id).text(data);


            });
        }
    });
}