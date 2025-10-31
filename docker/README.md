## Install and run ##
    
    For build docker image:
    docker build --tag netcat-alpine .

    Create bridge network:
    docker network create hw-network

    Run server container
    docker run -d --name server-container --network hw-network -v "$(pwd)":/scripts netcat-alpine tail -f /dev/null

    Run client container:
    docker run -d --name client-container --network hw-network -v "$(pwd)":/scripts netcat-alpine tail -f /dev/null
    
    In next steps we need to activate debug mod in bouth our containers:
    
    for client contaiter:
    docker exec -it client-container sh
    
    for server contaiter:
    docker exec -it server-container sh
    
    Now we can verify connections between containers, for this follow this commands:
    
    - from client container:
    ping 172.18.0.2
    
     - from server container:
    ping 172.18.0.3
    
    Finally we can run our srcips and see result:
    
    for server: 
    sh /scripts/server.sh
    
    for client:
    sh /scripts/client.sh
    
    Commands to verify netcat scripts:
    docker logs server-container
