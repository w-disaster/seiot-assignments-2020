const chartColor = '0, 149, 255';
const nMeasurements = 20;

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
        pointHoverRadius: 10,xAxesID: 'Time'
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

Number.prototype.allertToStatus = function(){
    status = "";
    color = "";

    switch (this.valueOf()) {
        case 0:
            status = "NORMALE";
            color = "primary";
            break;
        case 1:
            status = "PRE-ALLARME";
            color = "warning";
            break;
        case 2:
            status = "ALLARME";
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

//! FOR TESTING
Date.prototype.toXLabel = function() {
    return String(this.getDate()).padStart(2, '0') + "/" 
    + String(this.getMonth() + 1).padStart(2, '0') + "-" 
    + String(this.getHours()) + ":"
    + String(this.getMinutes()) + ":"
    + String(this.getSeconds());    
}
//!------------

function findLvl(className) {
    return className.includes("lvl-");
}

const id = ['state', 'w-lvl', 'd-level', 'context'];
const label = ['Dam status', 'Water level', 'Dam level', 'Dam Control Mode'];

$(function(){
    //! FOR TESTING
    let data_section = "";
    for(let i = 0; i < id.length; i++){
        data_section = data_section + 
            '<div id="' + id[i] + '" class="data py-3 lvl-' + i + '">' +
            '<p>' + label[i] + '</p>' + 
            '<p class="value fw-bold fs-3"></p></div>';
    }
    $("#data-section").html(data_section);
        
    //! questi valori andranno presi da DS
    let allertLevel = 0;
    $("div#state").after('<button>Change status</button>');
    $("button").on("click", function(){
        allertLevel = Math.floor(Math.random()* 3);
        
        $("div.data").each(function() {
            showLvl = $(this).attr("class").split(/\s+/).find(findLvl).split("-")[1];
            if(showLvl > allertLevel){
                $(this).hide();
            }else{
                $(this).show();
            }
            });

        if(allertLevel == 0){
            $("div#data-section").removeClass("col-md-5");
        }else{
            $("div#data-section").addClass("col-md-5");
        }
        
        updateStatus();
    });
    let waterLevel = 0;
    let damLevel = 100; 
    let context = "Automatic";

    //!------------

    function updateStatus(){
        const status = allertLevel.allertToStatus().status;
        const color = allertLevel.allertToStatus().color;

        // set values
        $("div#state > p.value").html('<span class="text-'+color+'">' + status+'</span>');
    }

    function updateWaterLevel(){
        $("div#w-lvl > p.value").html("<span class='text-primary'>"+waterLevel +" %</span>");
    }

    function updateDamLevel(){
        $("div#d-lvl > p.value").html(damLevel +" %");
    }

    function updateContext() {
        $("div#context > p.value").html(context);
    }
    
    updateStatus();
    updateWaterLevel();
    updateDamLevel();
    updateContext();

    $("div.data").each(function() {
       showLvl = $(this).attr("class").split(/\s+/).find(findLvl).split("-")[1];
       if(showLvl > allertLevel){
           $(this).hide();
       }else{
           $(this).show();
       }
    });

    if(allertLevel == 0){
        $("div#data-section").removeClass("col-md-5");
    }else{
        $("div#data-section").addClass("col-md-5");
    }

    //! FOR TESTING
    for (let i = 0; i < 20; i++) {
        data.labels.push(new Date().toXLabel()); // adds X    
        waterLevel = Math.floor(Math.random()* 101);
        updateWaterLevel();
        data.datasets[0].data.push(waterLevel); // adds Y

    }
    //!------------

    //canvas element
    var ctx = document.getElementById('chart').getContext('2d');

    // chart creation
    var myChart = new Chart(ctx, {
        type: 'line',
        data: data,
        options: options
    });
});

