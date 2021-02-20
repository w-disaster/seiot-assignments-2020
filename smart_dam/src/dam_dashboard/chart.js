// constants
const CHART_COLOR = '0, 149, 255';
const N_MEASURMENTS = 20;
const SERVER = 'http://localhost:8080/api/data';
const ALARM_REQUEST_TIME = 5000;
const PRE_ALARM_REQUEST_TIME = 10000;

// global variables
let requestTimeInterval = 5000;
let lastMesurmentReceived  = Date.now(); 

//data sets ( labels -> x | data -> y )
var data = {
    labels:[],
    datasets: [{
        label: 'Level of Water',
        lineTension: 0,
        data: [],
        backgroundColor: 'rgba('+ CHART_COLOR +', 0.2)',
        borderColor: 'rgba('+ CHART_COLOR +', 1)',
        borderWidth: 1,
        pointHoverRadius: 10,
    }],
};

//options mostly style
var options = {
    scales: {
        yAxes: [{
            ticks: {
                max:100,
                min:0,
                beginAtZero: true
            },
            scaleLabel: {
                display: true,
                labelString: 'Water Level Percentage'
                }
        }],
        xAxes: [{
            ticks: {
                beginAtZero: false
            },
            scaleLabel: {
                display: true,
                labelString: 'Time of Measurement'
            }
        }]
    }
};

function getAlertLevel(statusName){

    switch (statusName) {
        case "NORMAL":
            return 0;
        case "PRE-ALLARM":
            return 1;
        case "ALLARM":
            return 2;
        default:
            break;
    }
}

// sets from number to string
function alertToStatus(alert){
    status = "";
    color = "";

    switch (Number(alert)) {
        case 0:
            status = "NORMAL";
            color = "primary";
            break;
        case 1:
            status = "PRE-ALLARM";
            color = "warning";
            break;
        case 2:
            status = "ALLARM";
            color = "danger";
            break;
        default:
            status = "Errore";
            color = "danger";
            break;
    }
    
    return {
        status:status,
        color:color
    };
}

function stringToMode(mode){
    context = "";
    color = "";

    switch (mode) {
        case "AUTOMATIC":
            context = "AUTOMATIC";
            color = "success";
            break;
        case "MANUAL":
            context = "MANUAL";
            color = "warning";
            break;

        default:
            context = "Errore";
            color = "danger";
            break;
    }
    
    return {
        context:context,
        color:color
    };
}

function findLvl(className) {
    return className.includes("lvl-");
}

$(function(){

    function updateStatus(alert){
        const status = alertToStatus(alert);

        // set values
        $("div#state > p.value").html('<span class="text-'+status.color+'">' + status.status+'</span>');
    }

    function updateWaterLevel(waterLevel){
        $("div#w-lvl > p.value").html("<span class='text-primary'>"+waterLevel +" %</span>");
    }

    function updateDamLevel(damLevel){
        $("div#d-lvl > p.value").html(damLevel +" %");
    }

    function updateControlMode(mode) {
        const cont = stringToMode(mode);

        $("div#context > p.value").html('<span class="text-'+cont.color+'">' + cont.context+'</span>');
    }
    
    function addData(time, waterLevel){
        chart.data.labels.push(time); // adds X
        chart.data.datasets[0].data.push(waterLevel); // adds Y

        if(chart.data.labels.length > N_MEASURMENTS){
            chart.data.labels.shift(); // removes first X if over nRil
        }  
        
        if(chart.data.datasets[0].data.length > N_MEASURMENTS){
            chart.data.datasets[0].data.shift(); // removes first Y if over nRil
        }

        //update UI
        chart.update();
    }
    
    //canvas element
    var ctx = document.getElementById('chart').getContext('2d');

    // chart creation
    const chart = new Chart(ctx, {
        type: 'line',
        data: data,
        options: options
    });

    function processJSON(message){

        // alert level from message
        alert = getAlertLevel(message.damState);

        //* NORMAL
        updateStatus(alert);
        
        //filter what to show
        $("div.data").each(function() {
            //get the level of alert at which to show the div using the classes lvl-n
            showLvl = $(this).attr("class").split(/\s+/).find(findLvl).split("-")[1];

            if (showLvl > alert) {
                $(this).hide();
            } else {
                $(this).show();
            }
        });

        //adjust text center
        if (alert == 0) {
            $("div#data-section").removeClass("col-md-5");
        } else {
            $("div#data-section").addClass("col-md-5");
        }

        //? PRE-ALLARM
        if(alert > 0){
            //update water level
            updateWaterLevel(message.waterLevel);
            
            //update chart 
            for(let i = 0; i < message.measurments.length; i++){
                addData(message.measurments.time, message.measurments.waterLevel);
            }

            // update request interval on alert level basis
            requestTimeInterval = PRE_ALARM_REQUEST_TIME;

            //! ALLARM
            if(alert > 1){
                //update dam level
                updateDamLevel(message.damLevel);

                //update context
                updateControlMode(message.mode);
            
                // update request interval on alert level basis
                requestTimeInterval = ALARM_REQUEST_TIME;

            }
        }
    }

    var interval = setInterval(function(){
        // request last timestamp and
        var messageToServer = { Timestamp : lastMesurmentReceived };

        $.ajax({
            type: "GET",
            async: true,
            url: SERVER,
            dataType: "json",
            data: messageToServer,
            success: function(data) {
                let message = JSON.parse(data.data);
                processJSON(message);
                lastMesurmentReceived = data.Timestamp;
            },
            error: function(error) {
                console.log("ERROR: " + error);
            }
          });
    }, requestTimeInterval);
});

