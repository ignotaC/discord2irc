ircconf='../../data/irc.conf'

domain=` ./lext -c 'domain:' '' < "$ircconf" | tr -d '[:space:]' ` 
port=` ./lext -c 'port:' '' < "$ircconf" | tr -d '[:space:]' ` 
channel=` ./lext -c 'channel:' '' < "$ircconf" | tr -d '[:space:]' ` 
password=` ./lext -c 'password:' '' < "$ircconf" | tr -d '[:space:]' ` 
# you can't have spaces in password ;-)

nick=` ./lext -c 'nick:' '' < "$ircconf" | tr -d '[:space:]' ` 
hostname=` hostname `
servername=` uname `
realname=` ./lext -c 'realname:' '' < "$ircconf" | tr -d '[:space:]' ` 
userstr="$nick"' '"$hostname"' '"$servername"' '"$realname"

IP=` ./gethostipv "$domain" | head -n 1 `

./ircbot "$IP" "$port" "$channel" "$userstr" "$nick" "$password"
