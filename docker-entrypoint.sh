#!/bin/bash
set -e

# If cloud environment variables are present, generate config/database.json
if [ -n "$DB_HOST" ]; then
  mkdir -p config
  cat <<EOF > config/database.json
{
  "host": "${DB_HOST}",
  "port": ${DB_PORT:-3306},
  "database": "${DB_NAME:-defaultdb}",
  "username": "${DB_USER:-avnadmin}",
  "password": "${DB_PASSWORD:-}",
  "connectionPoolSize": 5,
  "connect_timeout_seconds": 15
}
EOF
  echo "[INFO] Generated config/database.json from cloud environment variables for host: ${DB_HOST}"
fi

# Launch the Smart Rental Management System backend
exec ./SmartRentalManagement

