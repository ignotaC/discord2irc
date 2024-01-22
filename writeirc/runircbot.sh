ircconf='../../data/irc.conf'

domain=` lext -c 'domain:' '' < "$ircconf" | tr -d '[:space:]' ` 
port=` lext -c 'port:' '' < "$ircconf" | tr -d '[:space:]' ` 
channel=` lext -c 'channel:' '' < "$ircconf" | tr -d '[:space:]' ` 

IP=` gethostipv "$domain" | head -n 1 `

./ircbot "$IP" "$port" "$channel"
