#!/bin/bash

if [ $# -lt 2 ]
	then
		echo "usage: sudo ./setup-autorun.sh <scriptWantToRun> <scriptNameInService>"
		exit
fi

autorunScript=$1
scriptInBin=$2

echo $autorunScript
echo $scriptInBin

# Step 1. create script in /usr/bin/
echo "#!/bin/bash" >> /usr/bin/$scriptInBin
echo $autorunScript >> /usr/bin/$scriptInBin
chmod a+x /usr/bin/$scriptInBin

# Step 2. create service
serviceFile=/lib/systemd/$scriptInBin.service

echo "[Unit]" >> $serviceFile 
echo "Description=run" >> $serviceFile 
echo "After=syslog.target network.target" >> $serviceFile 
echo "[Service]" >> $serviceFile 
echo "Type=simple" >> $serviceFile 
echo "ExecStart=/usr/bin/$scriptInBin" >> $serviceFile 
echo "[Install]" >> $serviceFile 
echo "WantedBy=multi-user.target" >> $serviceFile 

# Step 3. create symbolic link to 
ln $serviceFile /etc/systemd/system/$scriptInBin.service

# Step 4. start service
systemctl daemon-reload
systemctl start $scriptInBin.service
systemctl enable $scriptInBin.service
