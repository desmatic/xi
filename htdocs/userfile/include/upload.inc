<div id="uploadfiles" style="min-height: 26em; margin: 1em auto;">
    <h2 style="position: absolute; left: 35%; top: 30%; opacity: 0.2;">&nbsp;&nbsp;&nbsp;&nbsp;drag &amp; drop files here<br />(right click on files to share)<h2>
    <ul id="uploadfileslist">
    </ul>
</div>
<script type="text/javascript">

my.file.uploadfiles('uploadfiles', 'uploadfileslist');

/* add right click actions to uploadfileslist */
jQuery("#uploadfileslist").contextMenu({
    selector: 'li', 
    callback: function(key, options) {
        if (key == "trash") {
            my.file.trashfile(this[0]);
        } else if (key == "detail") {
            my.file.filedetails(this[0]);
        }
    },
    items: {
        "email": {name: "Email", icon: "email"},
        "share": {name: "Share", icon: "share"},
        "trash": {name: "Trash", icon: "trash"},
        "sep1": "---------",
        "detail": {name: "File Details"}
    }
});

/* add existing files to list */
<?php

    if ($rows = user_filedrive::user_files(session::userid())) {
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
