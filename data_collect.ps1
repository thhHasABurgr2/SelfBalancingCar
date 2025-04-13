
$serialPort = "COM6"


$port = New-Object System.IO.Ports.SerialPort
$port.PortName = "COM6"
$port.BaudRate = "9600"
$port.Parity = "None"
$port.DataBits = 8
$port.StopBits = 1
$port.DtrEnable = "true"
try {
    $port.open()
    Write-Host "Port opened successfully on $port"
} catch {
    Write-Host "Error opening port: $_"
    exit
}


#Start-Sleep 2
$count=0





Write-Host "Enter R for data collection to excel csv, S for calibration, Q for exit"
$script:ready="S"
$port.WriteLine($script:ready)




$outputFile= "dataTrial0.csv"
"t,theta" | Out-File -Encoding utf8 -FilePath $outputFile 
while($true){
    if ([console]::KeyAvailable) {# nonblock io for keyboard presses
        $key = [console]::ReadKey($true)
        if ($key.KeyChar -eq "s") {
            $script:ready = "S"
            $port.WriteLine($script:ready)
        }
        elseif ($key.KeyChar -eq "r"){
            $script:ready = "R"
            $port.WriteLine($script:ready)
            if($count -ne 0){
                $outputFile=("dataTrial"+$count.ToString()+".csv")
                "t,theta" | Out-File -Encoding utf8 -FilePath $outputFile 
            }
            $count++

        }
        elseif ($key.KeyChar -eq "q"){
            $script:ready = "Q"
        }
    }
    


    if($script:ready -eq "S"){
        try{
            if($port.BytesToRead -gt 0){
                $line = $port.ReadLine()
                Write-Host $line
                
            }
        }catch{
            Write-Host "Error in S mode. Err $_"
        }
        

    }elseif ($script:ready -eq "R"){
        
        try{
            
            
            if( $port.BytesToRead -gt 0){
                $line=$port.ReadLine()
                
                if ($line.Trim() -eq "F"){
                    $script:ready = "S"
                    continue;
                }else{
                    Write-Host ($line)
                    $line | Out-File -Append -Encoding utf8 -FilePath $outputFile
  
                }
            }

        }catch{
            Write-Host "Error in R mode. Err $_"
            continue
        }
        
    
    }elseif ($script:ready -eq "Q"){
        Write-Host ("finished. scuffed data in "+$count.ToString()+" files.")
        Write-Host "in the format of dataTrialNUM.csv NUM starts at 0"
        $port.WriteLine("S")
        $port.Close()
        exit
    }
    Start-Sleep -Milliseconds 10

}






