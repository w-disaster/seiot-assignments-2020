// constants
const CHART_COLOR = '0, 149, 255';
const N_MEASURMENTS = 20;
const SERVER = 'http://localhost:8080/api/data';
const ALARM_REQUEST_TIME = 5000;
const PRE_ALARM_REQUEST_TIME = 10000;
const CONNECTING_REQUEST_TIME = 500;
const DAM_INFO = $("div.data");

// global variables
let requestTimeInterval = PRE_ALARM_REQUEST_TIME;
//let lastMesurmentReceived  = Date.now(); 
let lastMesurmentReceived = 0;
// timer for ajax requests
let timer;

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
        case "Allarme":
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
        case -1:
            status = "CONNECTING...";
            color = "dark";
            break;
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
        case "Automatic":
            context = "AUTOMATIC";
            color = "success";
            break;
        case "Manual":
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

    initUI();

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

    function initUI() {
        // reset the timer for ajax
        requestTimeInterval = CONNECTING_REQUEST_TIME;
        setRequestTimer();
        // reset to connecting status
        updateStatus(-1);
        //update UI shown info
        updateUI(0);
    }

    function updateUI(alert){
        //filter what to show
        DAM_INFO.each(function() {
            //get the level of alert at which to show the div using the classes lvl-n
            showLvl = $(this).attr("class").split(/\s+/).find(findLvl).split("-")[1];

            if (showLvl > alert) {
                $(this).hide();
            } else {
                $(this).show();
            }
        });

        //adjust text center
        if (alert < 1) {
            $("div#data-section").removeClass("col-md-5");
        } else {
            $("div#data-section").addClass("col-md-5");
        }
    }

    function processJSON(message){

        // alert level from message
        alert = getAlertLevel(message.State);
        
        // tells what to show before setting data
        updateUI(alert);
        
        //* NORMAL
        updateStatus(alert);    

        //? PRE-ALLARM
        if(alert > 0){
            
            //update water level
            updateWaterLevel(message.WaterLevel);
        
            //update chart
            addData(message.Timestamp, message.WaterLevel);
            
            if(requestTimeInterval != PRE_ALARM_REQUEST_TIME){
                // update request interval on alert level basis
                requestTimeInterval = PRE_ALARM_REQUEST_TIME;
                setRequestTimer();
            }
            
            //! ALLARM
            if(alert > 1){
                //update dam level
                updateDamLevel(message.DamOpening);

                //update context
                updateControlMode(message.DamMode);
            
                if(requestTimeInterval != ALARM_REQUEST_TIME){
                    // update request interval on alert level basis
                    requestTimeInterval = ALARM_REQUEST_TIME;
                    setRequestTimer();
                }
            }
        }

        lastMesurmentReceived = message.Timestamp;
    } 
    
    function setRequestTimer() {

        if(timer != undefined){
            clearInterval(timer);
        }

        timer = setInterval(function(){
            // request last timestamp and
            var messageToServer = { Timestamp : lastMesurmentReceived };

            $.ajax({
                type: "GET",
                async: true,
                url: SERVER,
                dataType: "json",
                data: messageToServer,
                success: function(json) {
                    let message = json;
                    console.log(message);
                    // for all measurments in the json we process
                    for(let i = 0; i < message.length; i++){
                        processJSON(message[i]);
                    }
                    //!for testing
                    if (message.length == undefined){
                        processJSON(message);
                    }
                    //!-------------
                },
                error: function(error) {
                    initUI();
                    console.log("ERROR: " + error);
                }
            });
        }, requestTimeInterval);
    }
    
});

