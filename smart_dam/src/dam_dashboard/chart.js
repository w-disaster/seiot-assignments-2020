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


$(function(){
    //! FOR TESTING
        
        //! questi valori andranno presi da DS
        let allertLevel = 2;
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

    
    function addData(time){
        chart.data.labels.push(time); // adds X
        chart.data.datasets[0].data.push(waterLevel); // adds Y

        if(chart.data.labels.length > nMeasurements){
            chart.data.labels.shift(); // removes first X if over nRil
        }  
        
        if(chart.data.datasets[0].data.length > nMeasurements){
            chart.data.datasets[0].data.shift(); // removes first Y if over nRil
        }

        chart.update();
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
    
    //canvas element
    var ctx = document.getElementById('chart').getContext('2d');

    // chart creation
    const chart = new Chart(ctx, {
        type: 'line',
        data: data,
        options: options
    });

    //! FOR TESTING
    setInterval(function() {   
        time = new Date().toXLabel(); 

        waterLevel = Math.floor(Math.random()* 101);
        updateWaterLevel();
        
        addData(time);
    },1000);
    //!------------
    
});

