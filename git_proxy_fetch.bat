git config --global http.proxy http://127.0.0.1:1080 
git config --global https.proxy http://127.0.0.1:1080

git fetch

git config --global --unset http.proxy
git config --global --unset https.proxy

pause
