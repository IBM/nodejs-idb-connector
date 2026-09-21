const fs = require('fs');
const path = require('node:path');

const tag = process.env.TAG_NAME;
const token = process.env.GITHUB_TOKEN;

if (!tag) {
    console.error("Must set TAG_NAME environment variable");
    process.exit(1);
}
if (!token) {
    console.error("Must set GITHUB_TOKEN environment variable");
    process.exit(1);
}

const headers = {
    "Authorization": `Bearer ${token}`,
    "X-GitHub-Api-Version": "2022-11-28",
    "Accept": "application/vnd.github+json",
};

(async function script() {
    const url = 'https://api.github.com/repos/IBM/nodejs-idb-connector/releases';
    let response = await fetch(url, {
        method: 'POST',
        body: JSON.stringify({
            tag_name: tag,
            generate_release_notes: true,
            draft: true,
        }),
        headers: headers,
    });

    if (!response.ok) {
        console.error(`ERROR: Getting release failed: ${response.status}`)
        console.error(`URL: ${tag_url}`);
        process.exit(1);
    }

    const release_obj = await response.json();

    try {
        // Returns a goofy URL "template", get rid of the parameters
        const upload_url = release_obj.upload_url.substr(0, release_obj.upload_url.indexOf('{'));;

        const file = "lib/db2ia.node";
        const name = "db2ia.node";
        console.log(`Uploading ${file} -> ${name}`);

        const file_upload_url = `${upload_url}?name=${name}`;
        response = await fetch(file_upload_url, {
            method: "POST",
            body: fs.readFileSync(file),
            headers: {
                "Content-Type": "application/octet-stream",
                ...headers,
            }
        });
    }
    catch (e) {
        console.error(e);
        console.error('Deleting release');
        const response = await fetch(release_obj.url, {
            method: 'DELETE',
            headers: headers,
        });

        if (!response.ok) {
            console.error(`Failed: ${response.status} ${response.statusText}`);
        }

        process.exit(1);
    }

    if (!response.ok) {
        console.error(`ERROR: Upload of ${file} failed: ${response.status}`);
        console.error(`URL: ${file_upload_url}`);

        console.error('Deleting release');
        const response = await fetch(release_obj.url, {
            method: 'DELETE',
            headers: headers,
        });

        if (!response.ok) {
            console.error(`Failed: ${response.status} ${response.statusText}`);
        }

        process.exit(1);
    }

    response = await fetch(release_obj.url, {
        method: 'PATCH',
        headers: headers,
        body: JSON.stringify({
            draft: false,
        }),
    });

    if (!response.ok) {
        console.error(`ERROR: Couldn't update release draft status`);
        console.error(`URL: ${release_obj.url}`);
        process.exit(1);
    }
})();
