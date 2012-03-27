=============
Common Issues
=============


MySQL Error: (1062: 23000) Duplicate entry '8589934603' for key 'PRIMARY'
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This issue generally occurs when a configuration file from a previous, older mysql installation is still hanging around. The easiest fix is to uninstall MySQL and then manually remove the  **C:/Program Files/MySQL directory**. Once the old installation has been completely removed, reinstall the latest version of MySQL.
