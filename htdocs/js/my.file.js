/********************
 *
 * Author:      Des Jones
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.00
 *
 *****************************************/

var my = my || {};
my.file =  {
    
    trashfile: function(fileNode) {
        my.ajax.html(
            '/ajax.html', 
            { "action": "controller_userfile.trashfile", "user_filedriveid": fileNode.dataset.user_filedriveid }, 
            function(responseText) {
                fileNode.parentNode.removeChild(fileNode);
            }
        );
    },
    
    deletefile: function(fileNode) {
        my.ajax.html(
            '/ajax.html', 
            { "action": "controller_userfile.deletefile", "user_filedriveid": fileNode.dataset.user_filedriveid }, 
            function(responseText) {
                fileNode.parentNode.removeChild(fileNode);
            }
        );
    },

    deleteallfiles: function(fileNode) {
        my.ajax.html(
            '/ajax.html', 
            { "action": "controller_userfile.deleteallfiles" }, 
            function(responseText) {
                fileNode.parentNode.innerHTML = '';
            }
        );
    },
    
    restorefile: function(fileNode) {
        my.ajax.html(
            '/ajax.html', 
            { "action": "controller_userfile.restorefile", "user_filedriveid": fileNode.dataset.user_filedriveid }, 
            function(responseText) {
                fileNode.parentNode.removeChild(fileNode);
            }
        );
    },

    listuploadedfile: function(filelistId, file) {
        var extension = "";
        var filename = file.filename;
        var newchildNode = null;
        var parentlistNode = document.getElementById(filelistId);
        var uniqueId = my.uniqueId();     /* generate a uniqueId for each file */
        var fileurl = '';
        
        /* construct download url */
        if (file.md5sum && file.bytes && file.filename) {
            fileurl = '/getfile/userfile/' + file.md5sum + '/' + file.bytes + '/' + file.filename;
        }
        
        /* format file name */
        if (filename.lastIndexOf(".") > 0) {
            extension = filename.substring(filename.lastIndexOf("."));
            filename = filename.substring(0, filename.lastIndexOf("."));
        }
        filename = filename.length > 9 ? filename.substring(0, 8) + '~' : filename;
        filename += extension;
        
        /* add element to document */
        newchildNode = document.createElement('li');
        newchildNode.id = uniqueId + 'li';
        newchildNode.className = "superbox-list";
        newchildNode.dataset.user_filedriveid = file.user_filedriveid;
        /*
         * drag n drop downloads only work without cookies, so no authentication
         *
         *   newchildNode.addEventListener("dragstart", function(evt) {
         *       evt.dataTransfer.setData("DownloadURL", file.mimetype + ':' + file.filename + ':' + fileurl);
         *   }, false)
         */
        newchildNode.innerHTML = 
            '<a id="' + uniqueId + 'href" href="' + fileurl + '" draggable="true" download="' + file.filename + '" title="' + file.filename + ' / ' + file.datecreated + '" alt="' + file.filename + '" target="_blank">' +
                '<img id="' + uniqueId + 'image" class="superbox-img" alt="' + file.filename + '" src="/img/filetype/' + file.extension + '.png"></img>' + 
            '</a>' +
            '<p class="breakword smallest">' + filename + '</p>' +
            '<div class="progress-xs progress-striped">' +
                '<div id="' + uniqueId + '" class="progress-bar bg-color-red" style="width: ' + file.progressbar + '%;" role="progressbar"></div>' +
            '</div>';
        parentlistNode.appendChild(newchildNode);
        
        return uniqueId;
    },

    uploadfiles: function (dragndropId, filelistId) {
        var uploadfiles = document.getElementById(dragndropId);
        uploadfiles.ondragover = function() { return false; };
        uploadfiles.ondragend = function() { return false; };
        uploadfiles.ondrop = function(e) {
            var i = 0;
            var uniqueId = null;
            var formData = [];
            var request = [];
            var json = {};

            e.preventDefault();
            for (i = 0; i < e.dataTransfer.files.length; i++) {
                /* add file to list */
                uniqueId = my.file.listuploadedfile(filelistId, { "filename": e.dataTransfer.files[i].name, "extension": "tmp", "progressbar": 3 });
                
                /* post file */
                formData[i] = new FormData();
                formData[i].append('file', e.dataTransfer.files[i]);
                request[i] = new XMLHttpRequest();
                request[i].open('POST', '/ajax.html?action=controller_userfile.savefile');
                (function(uniqueId) {
                    request[i].onload = function() {
                        if (this.status == 200) {
                            if (json = JSON.parse(this.responseText)) {
                                document.getElementById(uniqueId + 'image').src = '/img/filetype/' + json.extension + '.png';
                                document.getElementById(uniqueId + 'href').href = '/getfile/userfile/' + json.md5sum + '/' + json.bytes + '/' + json.filename;
                                document.getElementById(uniqueId + 'li').dataset.user_filedriveid = json.user_filedriveid;
                            }
                        } else {
                            alert("Oops, something went wrong.");
                        }
                    };
                }(uniqueId));
                (function(uniqueId) {
                    request[i].upload.onprogress = function(event) {
                        if (event.lengthComputable) {
                            document.getElementById(uniqueId).style.width = Math.round(event.loaded / event.total * 100) + '%';
                        }
                    };
                }(uniqueId));
                request[i].send(formData[i]);
            }
        }
    },
    
    filedetails: function(fileNode) {
        my.ajax.window(
            '/ajax.html', 
            {
                action: "controller_userfile.filedetails",
                user_filedriveid: fileNode.dataset.user_filedriveid
            },
            'File Details',
            fileNode
        );
    }
};