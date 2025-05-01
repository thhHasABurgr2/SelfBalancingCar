<<<<<<< HEAD

=======
$serialPort = "COM6"
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af


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
$count=0
Start-Sleep -Milliseconds 2000 





$uiDir = Read-Host "name of directory"
$dir ="D:\lwcat\Documents\cStuff\"+$uiDir
New-Item -ItemType Directory -Path $dir -Force | Out-Null
$outputFile =Join-Path $dir ("dataTrial0.csv")
"t,omega1,omega2" | Out-File -Encoding utf8 -FilePath $outputFile

Write-Host "Enter A for 2 tests. Step input and natural dyanmics with ICs"
<<<<<<< HEAD
=======
$script:ready="A"
$port.WriteLine($script:ready)
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af


while($true){
    if ([console]::KeyAvailable) {# nonblock io for keyboard presses
        $key = [console]::ReadKey($true)
        if ($key.KeyChar -eq "a"){
            $script:ready = "A"
            $port.WriteLine($script:ready)
            if($count -ne 0){
                $outputFile= Join-Path $dir ("dataTrial{0}.csv" -f $count.ToString())
               "t,omega1,omega2"| Out-File -Encoding utf8 -FilePath $outputFile 
            }
            $count++

        }
        elseif ($key.KeyChar -eq "q"){
            $script:ready = "Q"
        }
    }
        if ($script:ready -eq "A"){
        
        try{
            
            
            if( $port.BytesToRead -gt 0){
                $line=$port.ReadLine()
                
                if (($line.Trim() -eq "F") -or ($line.Trim() -eq "starting step input test") -or ($line.Trim() -eq "natural dynamics from ICs") ){
                    Write-Host ($line)
                    continue
                }
                else{
                    Write-Host ($line)
                    $line | Out-File -Append -Encoding utf8 -FilePath $outputFile
  
                }
            }

        }catch{
            Write-Host "Error in A mode. Err $_"
            continue
        }
        
    
    }elseif ($script:ready -eq "Q"){
        Write-Host ("finished. scuffed data in "+$count.ToString()+" files.")
        Write-Host "in the format of dataTrialNUMDATE.csv NUM starts at 0"
        $port.Close()
        exit
    }
    Start-Sleep -Milliseconds 55

}