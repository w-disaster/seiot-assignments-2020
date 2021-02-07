<?php
header("Content-Type: text/event-stream");


$wLvl = rand(0, 100);
$dLvl = 100;
$cont = 0;
$allert = 1;

$counter = 0; // a random counter
while (1) {

    // to simulate event changing
    if (!$counter) {
        $counter = rand(5, 25);
        $allert = rand(0, 2);
        $cont = 0;
    }

    $counter--;

    echo "event: ping\n", '', "\n\n";

    // Send a simple message at const intervals.
    switch ($allert) {
        case 0:
            echo 'data: {"allertLevel":"' . $allert . '"}', "\n\n";
            break;

        case 1:
            $wLvl = rand(0, 100);
            echo 'data: {"time":"' . date("m/d-H:i:s") . '","waterLevel": "' . $wLvl . '","allertLevel":"' . $allert . '"}', "\n\n";
            break;

        case 2:
            $wLvl = rand(0, 100);

            $dLvl = $dLvl + 20 * rand(-1, 1);

            if ($dLvl < 0 || $dLvl > 100) {
                $dLvl = 60;
            }

            if ($dLvl < 60) {
                $cont = 0;
            } else {
                $cont = 1;
            }
            echo 'data: {"time":"' . date("m/d-H:i:s") . '","waterLevel": "' . $wLvl . '","allertLevel":"' . $allert . '","damLevel":"' . $dLvl . '","context":"' . $cont . '"}', "\n\n";
            break;

        default:
            # code...
            break;
    }

    // flush the output buffer and send echoed messages to the browser
    while (ob_get_level() > 0) {
        ob_end_flush();
    }
    flush();

    // break the loop if the client aborted the connection (closed the page)
    if (connection_aborted()) break;

    // sleep for 1 second before running the loop again
    sleep(1);
}
