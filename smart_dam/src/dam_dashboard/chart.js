// constants
const chartColor = '0, 149, 255';
const nMeasurements = 20;
const server = 'http://localhost:8080';
const requestTimeInterval = 1000;
const lastMesurmentReceived  = 0; 

//data sets ( labels -> x | data -> y )
var data = {
    labels:[],
    datasets: [{
        label: 'Level of Water',
        lineTension: 0,
        data: [],
        backgroundColor: 'rgba('+chartColor+', 0.2)',
        borderColor: 'rgba('+chartColor+', 1)',
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

function allertToStatus(allert){
    status = "";
    color = "";

    switch (Number(allert)) {
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

function idToContext(idContext){
    context = "";
    color = "";

    switch (Number(idContext)) {
        case 0:
            context = "AUTOMATIC";
            color = "success";
            break;
        case 1:
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

    function updateStatus(allert){
        const status = allertToStatus(allert);

        // set values
        $("div#state > p.value").html('<span class="text-'+status.color+'">' + status.status+'</span>');
    }

    function updateWaterLevel(waterLevel){
        $("div#w-lvl > p.value").html("<span class='text-primary'>"+waterLevel +" %</span>");
    }

    function updateDamLevel(damLevel){
        $("div#d-lvl > p.value").html(damLevel +" %");
    }

    function updateContext(idContext) {
        const cont = idToContext(idContext);

        $("div#context > p.value").html('<span class="text-'+cont.color+'">' + cont.context+'</span>');
    }
    
    function addData(time, waterLevel){
        chart.data.labels.push(time); // adds X
        chart.data.datasets[0].data.push(waterLevel); // adds Y

        if(chart.data.labels.length > nMeasurements){
            chart.data.labels.shift(); // removes first X if over nRil
        }  
        
        if(chart.data.datasets[0].data.length > nMeasurements){
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

    // SockJS websocket to connect to server on localhost:8080
    const sock = new SockJS(server);

    //! TESTING
    sock.onopen = function(){
        console.log("connected");
    }
    //!-------
    
    sock.onmessage = function(e) {
        let message = JSON.parse(e.data);
        processJSON(message);
    }

    function processJSON(message){
        //* NORMAL
        updateStatus(message.allertLevel);
        
        //filter what to show
        $("div.data").each(function() {
            //get the level of allert at which to show the div using the classes lvl-n
            showLvl = $(this).attr("class").split(/\s+/).find(findLvl).split("-")[1];

            if (showLvl > message.allertLevel) {
                $(this).hide();
            } else {
                $(this).show();
            }
        });

        //adjust text center
        if (message.allertLevel == 0) {
            $("div#data-section").removeClass("col-md-5");
        } else {
            $("div#data-section").addClass("col-md-5");
        }

        //? PRE-ALLARM
        if(message.allertLevel > 0){
            //update water level
            updateWaterLevel(message.waterLevel);
            
            //update chart   
            addData(message.time, message.waterLevel);

            //! ALLARM
            if(message.allertLevel > 1){
                //update dam level
                updateDamLevel(message.damLevel);

                //update context
                updateContext(message.context);
            }
        }
    }

    var interval = setInterval(function(){
        //TODO: send the request to server
        var messageToServer = { request : Date.now()};

        $.ajax({
            url: server,
            data: messageToServer,
            success: function(data) {
                let message = JSON.parse(data);
                processJSON(message);
            }
          });

        //sock.send("request" + Date.now());
        console.log("sent request" + Date.now());
    }, requestTimeInterval);
});

