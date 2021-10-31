<p class="right"><img src="http://www.w3.org/Icons/valid-xhtml10" alt="Valid XHTML 1.0 Strict" height="31" width="88" /></p>
<div class="logincentertop">
    <div class="logincenterleft">
    <form method="post" autocomplete="off">
        <div>
            <input type="hidden" name="action" value="controller.login" />
            <input type="hidden" name="view" value="login" />
            <input type="hidden" name="controller" value="<?= get_class($this); ?>" />
            <input type="hidden" name="page" value="<?= $this->get_page_request(); ?>" />
            <input type="hidden" name="get" value="<?= $this->get_page_request_variables(); ?>" />
            <input type="hidden" name="user[companyid]" value="1" />
        </div>
        <table class="table">
            <tr>
                <td colspan="2"><h2>Login</h2></td>
            </tr>
            <tr>
                <td><label for="useremail" <?php if (!empty($model['user']['email']['error'])) echo "class=\"lerror\""; ?>>Username</label></td>
                <td><input type="text" name="user[email]" id="useremail" value="<?= $model['user']['email']['value']; ?>" /></td>
            </tr>
            <tr>
                <td><label for="userpassword"  <?php if (!empty($model['user']['password']['error'])) echo "class=\"lerror\""; ?>>Password</label></td>
                <td><input type="password" name="user[password]" id="userpassword" value="<?= htmlentities($_REQUEST['user']['password']); ?>" /></td>
            </tr>
            <tr>
                <td colspan="2"><input type="submit" name="Login" value="Login" /></td>
            </tr>
        </table>
    </form>
    </div>
</div>
<script type="text/javascript">document.getElementById('useremail').focus()</script>