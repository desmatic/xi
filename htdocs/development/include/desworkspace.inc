<!-- Modal -->
<div class="modal fade" id="modal1" tabindex="-1" role="dialog" aria-labelledby="modal1Label" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->
<h1>
    <a data-toggle="modal" href="/ajax.html?action=<?= get_class($this); ?>.addimage" data-target="#modal1" title="Upload Image">
        <i class='fa fa-camera'></i>
    </a>
</h1>

<form id="file-form" action="handler.php" method="POST">
  <input type="file" id="file-select" name="photo"/>
  <button type="submit" id="upload-button">Upload</button>
</form>
<script type="text/javascript">

var form = document.getElementById('file-form');
var fileSelect = document.getElementById('file-select');
var uploadButton = document.getElementById('upload-button');

form.onsubmit = function(event) {
    event.preventDefault();

    // Update button text.
    uploadButton.innerHTML = 'Uploading...';

    var files = fileSelect.files;
    
    // Create a new FormData object.
    var formData = new FormData();
    
    // Loop through each of the selected files.
    for (var i = 0; i < files.length; i++) {
        var file = files[i];

        // Check the file type.
        if (!file.type.match('image.*')) {
            continue;
        }

        // Add the file to the request.
        formData.append('photos[]', file, file.name);
    }
    
    // Set up the request.
    var xhr = new XMLHttpRequest();
    
    // Open the connection.
    xhr.open('POST', '/ajax.php?action=controller_development.logrequest', true);
    
    // Set up a handler for when the request finishes.
    xhr.onload = function () {
        if (xhr.status === 200) {
            // File(s) uploaded.
            uploadButton.innerHTML = 'Uploaded!';
        } else {
            alert('An error occurred!');
        }
    };
    
    // Send the Data.
    xhr.send(formData);
}

</script>