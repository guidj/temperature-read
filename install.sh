PROGRAM_FILES=" config.json interval.py tempread tempread.py "
PROGRAM_DIR="/opt/TAG/tempread"
SERVICE_SCRIPT=$PROGRAM_DIR/tempread.py
INIT_SCRIPT="tempread.sh"
INIT_SCRIPT_DEST="/etc/init.d/tempread"

sudo scp $PROGRAM_FILES $PROGRAM_DIR/

sudo scp $INIT_SCRIPT $INIT_SCRIPT_DEST

sudo chmod 755 $INIT_SCRIPT_DEST
sudo chmod 755 $SERVICE_SCRIPT
