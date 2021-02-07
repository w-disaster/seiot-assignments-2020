<?php

define('MAX_ALLERT', 2);
define('N_RIL', 20);

# all data to show in DD
$params = array(
    array(
        "id" => "state",
        "label" => "Dam Status",
    ),
    array(
        "id" => "w-lvl",
        "label" => "Water Level",
    ),
    array(
        "id" => "d-lvl",
        "label" => "Dam Level",
    ),
    array(
        "id" => "context",
        "label" => "Dam Control Mode",
    ),
);

$alertLevelShow = 0;
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <!-- Bootsrap CSS only -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-giJF6kkoqNQ00vy+HMDP7azOuL0xtbfIcaT9wjKHr8RbDVddVHyTfAAsrekwKmP1" crossorigin="anonymous">

    <!-- style.css -->
    <link rel="stylesheet" href="style.css" />

    <!-- jQuery import -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>

    <!-- chart js library -->
    <script src="https://cdn.jsdelivr.net/npm/chart.js@2.9.4/dist/Chart.min.js"></script>

    <title>Dam Dashboard</title>
</head>

<body>
    <header class="text-center">
        <h1 class="py-5 col-12">Dam Dashboard</h1>
    </header>

    <main class="row">

        <div id="data-section" class="text-center col-12">
            <?php foreach ($params as $param) : ?>
                <div id="<?php echo $param["id"] ?>" class="data py-3 lvl-<?php echo $alertLevelShow ?>">
                    <p><?php echo $param["label"] ?></p>
                    <p class="value fw-bold fs-3"></p>
                </div>
                <?php
                $alertLevelShow = $alertLevelShow + 1;
                if ($alertLevelShow > MAX_ALLERT) {
                    $alertLevelShow = MAX_ALLERT;
                }
                ?>
            <?php endforeach ?>
        </div>

        <div id="chart-section" class="col-md-6 data lvl-1">
            <p class="text-center">Chart of the last <?php echo N_RIL ?> measurments</p>
            <canvas id="chart" class="img-responsive"></canvas>

            <!-- javascript chart import -->
            <script src="chart.js"></script>
        </div>

        <div class="col-md-1">
            <!--empty space-->
        </div>
    </main>
</body>

</html>