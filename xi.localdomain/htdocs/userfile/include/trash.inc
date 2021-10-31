<div id="uploadfiles" style="min-height: 26em; margin: 1em auto;">
    <h2 style="position: absolute; left: 35%; top: 30%; opacity: 0.2;">right click on files to restore<h2>
    <ul id="uploadfileslist">
    </ul>
</div>
<script type="text/javascript">

my.file.uploadfiles('uploadfiles', 'uploadfileslist');

/* add right click actions to uploadfileslist */
jQuery("#uploadfileslist").contextMenu({
    selector: 'li',
    callback: function(key, options) {
        if (key == "restore") {
            my.file.restorefile(this[0]);
        } else if (key == "delete") {
            my.file.deletefile(this[0]);
        } else if (key == "empty") {
            my.file.deleteallfiles(this[0]);
        }
    },
    items: {
        "restore": { name: "Restore", icon: "restore" },
        "delete": {name: "Delete From Trash", icon: "trash"},
        "sep1": "---------",
        "empty": { name: "Empty All Trash" }
    }
});

/* add existing files to list */
<?php

    if ($rows = user_filedrive::user_files_trashed(session::userid())) {
        foreach ($rows as $row) {
            $row["extension"] = filedrive::extension($row["mimetype"]);
            $row['mimetype'] = filedrive::abbreviate_mimetype($row["mimetype"]);
            $row["progressbar"] = 0;
            $file = json_encode((object) $row);
            
            echo "my.file.listuploadedfile('uploadfileslist', {$file});\n";
        }
    }

?>
</script>
